#include <iostream>
#include <wmmintrin.h>
#include <emmintrin.h>

#include "../include/FastAES.hpp"

/**
 * @brief Checks if the CPU supports AES hardware acceleration instructions.
 * 
 * @return true if AES-NI is supported, false otherwise.
 */
inline bool FastAES::supports_aes() const noexcept
{
    std::array<int, 4> arr;
    __asm__ __volatile__(
        "cpuid"
        : "=a"(arr[0]), "=b"(arr[1]), "=c"(arr[2]), "=d"(arr[3])
        : "a"(1)
    );

    return (arr[2] & (1<<25)) != 0;
}

/**
 * @brief Constructs a FastAES object and initializes key schedules for encryption and decryption.
 * 
 * @param key The encryption key as a 128-bit (16 bytes) array.
 */
FastAES::FastAES(const uint8_t* key) : key(key), enc_key_schedule(new uint8_t[176]), dec_key_schedule(new uint8_t[176])
{
    if (not supports_aes())
    {
        std::cerr << "This cpu doesnt supports AES hardware acceleration instruction set, aborting!\n";
        return;
    }

    key_expansion(enc_key_schedule.get(), dec_key_schedule.get());
}

FastAES::~FastAES()
{
}

/**
 * @brief Expands the encryption key into encryption and decryption key schedules.
 * 
 * @param enc_key_schedule Pointer to the memory where the encryption key schedule will be stored.
 * @param dec_key_schedule Pointer to the memory where the decryption key schedule will be stored.
 */
void FastAES::key_expansion(uint8_t* enc_key_schedule, uint8_t* dec_key_schedule) noexcept
{
    const auto& finish_rk = [](__m128i prev_rk, __m128i ass_rk) {
        ass_rk = _mm_shuffle_epi32(ass_rk ,0xff);
        __m128i tmp = _mm_slli_si128(prev_rk, 0x4);
        prev_rk = _mm_xor_si128(prev_rk, tmp);
        tmp = _mm_slli_si128(tmp, 0x4);
        prev_rk = _mm_xor_si128(prev_rk, tmp);
        tmp = _mm_slli_si128(tmp, 0x4);
        prev_rk = _mm_xor_si128(prev_rk, tmp);
        prev_rk = _mm_xor_si128(prev_rk, ass_rk);
        
        return prev_rk;
    };

    __m128i* enc_key_schedule_vec = reinterpret_cast<__m128i*>(enc_key_schedule);
    enc_key_schedule_vec[0] = _mm_load_si128(reinterpret_cast<const __m128i*>(key)); 
    enc_key_schedule_vec[1] = finish_rk(enc_key_schedule_vec[0], _mm_aeskeygenassist_si128(enc_key_schedule_vec[0], 0x1));
    enc_key_schedule_vec[2] = finish_rk(enc_key_schedule_vec[1], _mm_aeskeygenassist_si128(enc_key_schedule_vec[1], 0x2));
    enc_key_schedule_vec[3] = finish_rk(enc_key_schedule_vec[2], _mm_aeskeygenassist_si128(enc_key_schedule_vec[2], 0x4));
    enc_key_schedule_vec[4] = finish_rk(enc_key_schedule_vec[3], _mm_aeskeygenassist_si128(enc_key_schedule_vec[3], 0x8));
    enc_key_schedule_vec[5] = finish_rk(enc_key_schedule_vec[4], _mm_aeskeygenassist_si128(enc_key_schedule_vec[4], 0x10));
    enc_key_schedule_vec[6] = finish_rk(enc_key_schedule_vec[5], _mm_aeskeygenassist_si128(enc_key_schedule_vec[5], 0x20));
    enc_key_schedule_vec[7] = finish_rk(enc_key_schedule_vec[6], _mm_aeskeygenassist_si128(enc_key_schedule_vec[6], 0x40));
    enc_key_schedule_vec[8] = finish_rk(enc_key_schedule_vec[7], _mm_aeskeygenassist_si128(enc_key_schedule_vec[7], 0x80));
    enc_key_schedule_vec[9] = finish_rk(enc_key_schedule_vec[8], _mm_aeskeygenassist_si128(enc_key_schedule_vec[8], 0x1B));
    enc_key_schedule_vec[10] = finish_rk(enc_key_schedule_vec[9], _mm_aeskeygenassist_si128(enc_key_schedule_vec[9], 0x36));

    __m128i* dec_key_schedule_vec = reinterpret_cast<__m128i*>(dec_key_schedule);
    dec_key_schedule_vec[0] = enc_key_schedule_vec[0];
    for (int i = 1; i < 10; ++i)
        dec_key_schedule_vec[i] = _mm_aesimc_si128(enc_key_schedule_vec[i]);
    dec_key_schedule_vec[10] = enc_key_schedule_vec[10];
}

/**
 * @brief Encrypts data using AES in ECB mode.
 * 
 * @param src Pointer to the input data to be encrypted.
 * @param dest Pointer to the output buffer where encrypted data will be stored.
 * @param length Length of the data to be encrypted in bytes.
 * @param mode Encryption mode, currently only supports ECB.
 * @param num_threads Number of threads to use for encryption.
 */
void FastAES::encrypt(const uint8_t* src, uint8_t* dest, std::size_t length, uint32_t num_threads, const ENC_MODE mode) noexcept
{
    if (mode == ENC_MODE::ECB)
    {
        auto thread_func = [&](const uint8_t* _src, uint8_t* _dest, int start, int end, const uint8_t* enc_key_schedule_ptr)
        {
            for (int i = start; i < end; ++i)
            {
                const __m128i* _enc_key_schedule_vector = reinterpret_cast<const __m128i*>(enc_key_schedule_ptr);
                __m128i stage =  _mm_load_si128(reinterpret_cast<const __m128i*>(_src + 16*i));
                stage = _mm_xor_si128(stage, _enc_key_schedule_vector[0]);

                for (int j = 1; j < 10; ++j)
                    stage = _mm_aesenc_si128(stage, _enc_key_schedule_vector[j]);
                stage = _mm_aesenclast_si128(stage, _enc_key_schedule_vector[10]);
                
                _mm_store_si128(reinterpret_cast<__m128i*>(_dest + 16*i), stage);
            }
        };

        uint32_t iter = length / 16 + (length%16 != 0);
        num_threads = std::min(iter, num_threads);

        const int block_per_thread = iter / num_threads;
        const int remainder_blocks = iter % num_threads;

        int start = 0;
        std::vector<std::thread> threads;
        const uint8_t* enc_key_schedule_ptr = enc_key_schedule.get();
        for (int i = 0; i < num_threads; ++i)
        {
            int end = start + block_per_thread + (i < remainder_blocks ? 1 : 0);
            threads.emplace_back(thread_func, src, dest, start, end, enc_key_schedule_ptr);
            start = end;
        }

        for (auto &thread : threads)
            if (thread.joinable())
                thread.join();
    }
}

/**
 * @brief Decrypts data using AES in ECB mode.
 * 
 * @param src Pointer to the input data to be decrypted.
 * @param dest Pointer to the output buffer where decrypted data will be stored.
 * @param length Length of the data to be decrypted in bytes.
 * @param mode Decryption mode, currently only supports ECB.
 * @param num_threads Number of threads to use for decryption.
 */
void FastAES::decrypt(const uint8_t* src, uint8_t* dest, std::size_t length, uint32_t num_threads, const ENC_MODE mode) noexcept
{
    if (mode == ENC_MODE::ECB)
    {
        auto thread_func = [&](const uint8_t* _src, uint8_t* _dest, int start, int end, const uint8_t* dec_key_schedule_ptr)
        {
            for (int i = start; i < end; ++i)
            {
                const __m128i* _dec_key_schedule_vector = reinterpret_cast<const __m128i*>(dec_key_schedule_ptr);
                __m128i stage = _mm_load_si128(reinterpret_cast<const __m128i*>(_src + 16*i));
                stage = _mm_xor_si128(stage, _dec_key_schedule_vector[10]); 

                for (int j = 1; j <= 9; ++j)
                    stage = _mm_aesdec_si128(stage, _dec_key_schedule_vector[10 - j]);
                stage = _mm_aesdeclast_si128(stage, _dec_key_schedule_vector[0]);
                
                _mm_store_si128(reinterpret_cast<__m128i*>(_dest + 16*i), stage);
            }
        };

        uint32_t iter = length / 16 + (length%16 != 0);
        num_threads = std::min(iter, num_threads);

        const int block_per_thread = iter / num_threads;
        const int remainder_blocks = iter % num_threads;

        int start = 0;
        std::vector<std::thread> threads;
        const uint8_t* dec_key_schedule_ptr = dec_key_schedule.get();
        for (int i = 0; i < num_threads; ++i)
        {
            int end = start + block_per_thread + (i < remainder_blocks ? 1 : 0);
            threads.emplace_back(thread_func, src, dest, start, end, dec_key_schedule_ptr);
            start = end;
        }

        for (auto &thread : threads)
            if (thread.joinable())
                thread.join();
    }
}