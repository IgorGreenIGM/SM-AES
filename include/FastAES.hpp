#ifndef __FAST_AES_H_INCLUDED__
#define __FAST_AES_H_INCLUDED__

#include <array>
#include <vector>
#include <memory>
#include <thread>
#include <cstdint>

/**
 * @brief A class for fast AES encryption and decryption using hardware acceleration.
 * 
 * This class utilizes AES-NI (AES New Instructions) available in modern CPUs
 * for efficient AES encryption and decryption. It supports multithreading to
 * parallelize the encryption and decryption process.
 */
class FastAES
{
    private:
        const uint8_t* key = nullptr;
        std::unique_ptr<uint8_t> enc_key_schedule;
        std::unique_ptr<uint8_t> dec_key_schedule;

        void key_expansion(uint8_t* enc_key_schedule, uint8_t* dec_key_schedule) noexcept;

    public:
        ~FastAES();
        FastAES(const uint8_t* key);
        inline bool supports_aes() const noexcept;

        /**
         * @brief Enumeration for specifying the encryption mode.
         */
        enum class ENC_MODE {ECB};
        void encrypt(const uint8_t* src, uint8_t* dest, std::size_t length, uint32_t num_threads=std::thread::hardware_concurrency(), const ENC_MODE mode= ENC_MODE::ECB) noexcept;
        void decrypt(const uint8_t* src, uint8_t* dest, std::size_t length, uint32_t num_threads=std::thread::hardware_concurrency(), const ENC_MODE mode= ENC_MODE::ECB) noexcept;

};

#endif // __FAST_AES_H_INCLUDED__