#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <cstring>

#include "../include/FastAES.hpp"

std::vector<uint8_t> generate_random_data(size_t size)
{
    std::vector<uint8_t> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dis(0, 255);

    for (auto &byte : data)
        byte = dis(gen);

    return data;
}

void benchmark_aes(FastAES &aes, const uint8_t *data, size_t data_size, bool encrypt, uint32_t num_threads, double &mean_time, double &mean_rate)
{
    std::vector<uint8_t> output(data_size);
    std::vector<double> times;

    for (int run = 0; run < 10; ++run)
    {
        auto start = std::chrono::high_resolution_clock::now();

        if (encrypt)
            aes.encrypt(data, output.data(), data_size);
        else
            aes.decrypt(data, output.data(), data_size);

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
    std::vector<size_t> sizes = {512ULL * 1024 * 1024, 1ULL * 1024 * 1024 * 1024, 2ULL * 1024 * 1024 * 1024};
    uint8_t key[16] = {0xa1, 0xff, 0x03, 0x01,
                       0x00, 0x02, 0x05, 0xf2,
                       0x2A, 0xF3, 0xF4, 0xD4,
                       0xB1, 0x32, 0x4F, 0xDF};
    FastAES aes(key);

    for (size_t size : sizes)
    {
        std::cout << "Benchmarking for size: " << size / (1024 * 1024) << " MB" << std::endl;

        double encrypt_mean_time = 0.0;
        double encrypt_mean_rate = 0.0;
        double decrypt_mean_time = 0.0;
        double decrypt_mean_rate = 0.0;

        for (int i = 0; i < 10; ++i)
        {
            auto data = generate_random_data(size);

            benchmark_aes(aes, data.data(), size, true, std::thread::hardware_concurrency(), encrypt_mean_time, encrypt_mean_rate);
            benchmark_aes(aes, data.data(), size, false, std::thread::hardware_concurrency(), decrypt_mean_time, decrypt_mean_rate);

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
