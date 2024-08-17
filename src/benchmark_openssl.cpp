#include <openssl/evp.h>
#include <openssl/rand.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <cstring>
#include <numeric>

std::vector<uint8_t> generate_random_data(size_t size)
{
    std::vector<uint8_t> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 255);

    for (auto& byte : data)
        byte = static_cast<uint8_t>(dis(gen));

    return data;
}

void openssl_aes_encrypt(const uint8_t* plaintext, uint8_t* ciphertext, int size, const uint8_t* key, const uint8_t* iv)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len;
    int ciphertext_len;

    EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, size);
    ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
}

void openssl_aes_decrypt(const uint8_t* ciphertext, uint8_t* plaintext, int size, const uint8_t* key, const uint8_t* iv)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len;
    int plaintext_len;

    EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, iv);
    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, size);
    plaintext_len = len;
    EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
}

void benchmark_openssl_aes(const uint8_t* data, int data_size, bool encrypt, const uint8_t* key, double& mean_time, double& mean_rate)
{
    std::vector<uint8_t> output(data_size);
    std::vector<double> times;

    uint8_t iv[16] = { 0 }; 

    for (int run = 0; run < 10; ++run)
    {
        auto start = std::chrono::high_resolution_clock::now();

        if (encrypt)
            openssl_aes_encrypt(data, output.data(), data_size, key, iv);
        else
            openssl_aes_decrypt(data, output.data(), data_size, key, iv);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        double throughput = data_size / (1024.0 * 1024.0) / duration.count();

        times.push_back(duration.count());
        mean_rate += throughput;
    }

    mean_time = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
    mean_rate /= times.size();
}

int main()
{
    std::vector<size_t> sizes = { 512ULL * 1024 * 1024, 1ULL * 1024 * 1024 * 1024, 2ULL * 1024 * 1024 * 1024 };
    uint8_t key[16] = { 0xa1, 0xff, 0x03, 0x01,
                       0x00, 0x02, 0x05, 0xf2,
                       0x2A, 0xF3, 0xF4, 0xD4,
                       0xB1, 0x32, 0x4F, 0xDF };

    for (size_t size : sizes)
    {
        std::cout << "Benchmarking OpenSSL AES for size: " << size / (1024 * 1024) << " MB" << std::endl;

        double encrypt_mean_time = 0.0;
        double encrypt_mean_rate = 0.0;
        double decrypt_mean_time = 0.0;
        double decrypt_mean_rate = 0.0;

        for (int i = 0; i < 10; ++i)
        {
            auto data = generate_random_data(size);

            benchmark_openssl_aes(data.data(), size, true, key, encrypt_mean_time, encrypt_mean_rate);
            benchmark_openssl_aes(data.data(), size, false, key, decrypt_mean_time, decrypt_mean_rate);

            std::cout << "Run " << (i + 1) << ":\n";
            std::cout << "  Encryption - Mean Time: " << encrypt_mean_time << " seconds, Mean Rate: " << encrypt_mean_rate << " MB/s\n";
            std::cout << "  Decryption - Mean Time: " << decrypt_mean_time << " seconds, Mean Rate: " << decrypt_mean_rate << " MB/s\n";
        }

        std::cout << "Average Encryption Time: " << encrypt_mean_time << " seconds\n";
        std::cout << "Average Encryption Rate: " << encrypt_mean_rate << " MB/s\n";
        std::cout << "Average Decryption Time: " << decrypt_mean_time << " seconds\n";
        std::cout << "Average Decryption Rate: " << decrypt_mean_rate << " MB/s\n";
        std::cout << "--------------------------------------------------" << std::endl;
    }

    return 0;
}
