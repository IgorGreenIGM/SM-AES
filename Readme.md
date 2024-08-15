# SIMD-MT-AES: SIMD Multithreaded AES

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
![GitHub License](https://img.shields.io/badge/license-MIT-1006.svg)
![GitHub Release](https://img.shields.io/github/v/release/IgorGreenIGM/SM-AES)
![GitHub last commit](https://img.shields.io/github/last-commit/IgorGreenIGM/SM-AES)
![GitHub top language](https://img.shields.io/github/languages/top/IgorGreenIGM/SM-AES)

`SIMD-MT-AES` is a high-performance C++ AES encryption and decryption utility with hardware acceleration and multithreading support. It uses AES-NI for fast operations and is optimized for speed, supporting only AES-128 encryption in ECB mode.

## Features

- **AES-128 Encryption and Decryption** with AES-NI acceleration.
- **Multithreading Support** to leverage multiple CPU cores.
- **Command-Line Tool** for message and file encryption/decryption.
- **C++ Library** for integration into other projects.
- **Automatic Key Management** for proper key sizing.
- **PKCS5 Padding** for correct block alignment.
- **ECB Mode Only** support.

## Usage

### Clone the Repository

```bash
git clone https://github.com/IgorGreenIGM/SM-AES.git
cd SM-AES
```

### Command-Line Interface (CLI)

`SIMD-MT-AES` can be used as a command-line tool for encrypting and decrypting messages and files.

#### Command-Line Options

```bash
sm-aes.exe [options]
```

- `-enc` : Encrypt the input.
- `-dec` : Decrypt the input.
- `-key <key>` : Specify the encryption/decryption key.
- `-msg <text>` : Specify the plaintext message to encrypt.
- `-in <file>` : Specify the input file path.
- `-out <file>` : Specify the output file path.
- `-thd <thread number>` : Specify the number of threads.
- `-h` : Display the help message.

#### Examples

- **Encrypt a Message:**

  ```sh
  sm-aes.exe -enc -key mysecretkey123456 -msg "Hello, World!"
  ```

- **Decrypt a Message:**

  ```sh
  sm-aes.exe -dec -key mysecretkey123456 -msg c080664469a3770e8424cdd0e6bb9e21
  ```

- **Encrypt a File:**

  ```sh
  sm-aes.exe -enc -key mysecretkey123456 -in input.txt -out encrypted.bin -thd 4
  ```

- **Decrypt a File:**

  ```sh
  sm-aes.exe -dec -key mysecretkey123456 -in encrypted.bin -out decrypted.txt
  ```

### C++ Library Integration

`SIMD-MT-AES` can also be used as a C++ library.

#### Integration Instructions

- **Include Header:** Add the header file `FastAES.hpp` to your project.

#### Example Usage

- **Encrypting Data:**

  ```cpp
  #include "FastAES.hpp"

  uint8_t key[16] = { /* Your 16-byte key here */ };
  FastAES aes(key);

  const char* plaintext = "Hello, World!";
  size_t length = strlen(plaintext);
  std::vector<uint8_t> ciphertext(length + 16); 

  aes.encrypt(reinterpret_cast<const uint8_t*>(plaintext), ciphertext.data(), length);
  ```

- **Decrypting Data:**

  ```cpp
  #include "FastAES.hpp"

  uint8_t key[16] = { /* Your 16-byte key here */ };
  FastAES aes(key);

  std::vector<uint8_t> decrypted(length);
  aes.decrypt(ciphertext.data(), decrypted.data(), length);
  ```

## Building

### Using g++

```bash
g++ src/FastAES.cpp src/main.cpp -o bin/sm-aes.exe -maes -msse4 -m64 -O3 -std=c++11
```

### Using Make

```bash
make
```

## Benchmark

The benchmark results were obtained by measuring AES encryption and decryption for different data sizes.

### Results

| Size    | Operation   | Average Time  | Average Rate |
|---------|-------------|---------------|--------------|
| **512 MB** | Encryption  | 0.0946 seconds | 5995.53 MB/s |
|         | Decryption  | 0.1013 seconds | 5608.9 MB/s  |
| **1024 MB** | Encryption  | 0.1864 seconds | 6117.79 MB/s |
|         | Decryption  | 0.1902 seconds | 6022.73 MB/s |
| **2048 MB** | Encryption  | 0.3620 seconds | 6152.65 MB/s |
|         | Decryption  | 0.6100 seconds | 4474.26 MB/s |

### Benchmark Environment

- **CPU:** Intel(R) Core(TM) i7-4810MQ @ 2.80GHz, 4 cores, 8 threads
- **RAM:** 16 GB DDR3 @ 1600MHz
- **OS:** Windows 10 Professional (64-bit)
- **Compiler:** g++ (GCC) 6.1.0

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE.md) file for details.