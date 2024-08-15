# SIMD-MT-AES: SIMD Multithreaded AES
[![Status](https://img.shields.io/badge/status-active-success.svg)]()
![GitHub License](https://img.shields.io/badge/license-MIT-1006.svg)
![GitHub Release](https://img.shields.io/github/v/release/IgorGreenIGM/SM-AES)
![GitHub last commit](https://img.shields.io/github/last-commit/IgorGreenIGM/SM-AES)
![GitHub top language](https://img.shields.io/github/languages/top/IgorGreenIGM/SM-AES)

`SIMD-MT-AES` is a high-performance C++ AES encryption and decryption utility with support for hardware acceleration and multithreading. It utilizes AES-NI (AES New Instructions) for fast encryption and decryption, along with multithreading to boost performance. It is optimized for speed and supports only AES-128 encryption in ECB mode.

A high-performance C++ library implementing AES-128 encryption/decryption using SIMD, AESNI, and multithreading. Optimized for speed and efficiency, this library leverages hardware acceleration and parallel processing to achieve rapid cryptographic operations.


## Features

- **AES-128 Encryption and Decryption**: Uses AES-NI for accelerated performance.
- **Multithreading Support**: Leverages multiple CPU cores to speed up encryption and decryption.
- **Command-Line Tool**: Encrypts and decrypts messages and files from the command line.
- **C++ Library**: Can be integrated into other C++ projects for AES operations.
- **Automatic Key Management**: Keys are truncated or padded as necessary.
- **PKCS5 Padding Support**: Handles padding for proper block size alignment.
- **ECB Mode Only**: Currently supports only the Electronic Codebook mode of operation.

## Usage

### Command-Line Interface (CLI)

`SIMD-MT-AES` can be used as a command-line tool for encrypting and decrypting messages and files.

#### Command-Line Options

```
sm-aes.exe [options]
```

#### Options

- `-enc`  
  Encrypt the input.

- `-dec`  
  Decrypt the input.

- `-key <key>`  
  Specify the encryption/decryption key (keys larger than 16 bytes will be truncated, and smaller keys will be space-padded).

- `-msg <text>`  
  Specify the plaintext message to encrypt (output could contain extra bytes for padding).

- `-in <file>`  
  Specify the input file path for encryption/decryption.

- `-out <file>`  
  Specify the output file path (output could contain extra bytes for padding).

- `-thd <thread number>`  
  Specify the number of threads to use for the encryption/decryption operation. Defaults to the number of available CPU cores.

- `-h`  
  Display the help message and exit.

---

#### Examples

##### Encrypt a Message

Encrypt a plaintext message with a key and display the result:

```sh
sm-aes.exe -enc -key mysecretkey123456 -msg "Hello, World!"
```

##### Decrypt a Message

Decrypt an encrypted message with the same key and display the result:

```sh
sm-aes.exe -dec -key mysecretkey123456 -msg c080664469a3770e8424cdd0e6bb9e21
```

##### Encrypt a File

Encrypt a file with a key and specify the number of threads:

```sh
sm-aes.exe -enc -key mysecretkey123456 -in input.txt -out encrypted.bin -thd 4
```

##### Decrypt a File

Decrypt a file with the same key and save the output to a text file:

```sh
sm-aes.exe -dec -key mysecretkey123456 -in encrypted.bin -out decrypted.txt
```

---

### C++ Library Integration

`SIMD-MT-AES` can also be used as a C++ library for integrating AES encryption and decryption into your projects. 

#### Integration Instructions

**Include Header**: Add the header file `FastAES.hpp` to your project.

#### Example Usage

##### Encrypting Data

```cpp
#include "FastAES.hpp"

// Initialize the AES object with a key
uint8_t key[16] = { /* Your 16-byte key here */ };
FastAES aes(key);

// Encrypt a plaintext message
const char* plaintext = "Hello, World!";
size_t length = strlen(plaintext);
std::vector<uint8_t> ciphertext(length + 16); // Ensure enough space for padding

aes.encrypt(reinterpret_cast<const uint8_t*>(plaintext), ciphertext.data(), length);

// Use ciphertext.data() for the encrypted output
```

##### Decrypting Data

```cpp
#include "FastAES.hpp"

// Initialize the AES object with the same key
uint8_t key[16] = { /* Your 16-byte key here */ };
FastAES aes(key);

// Decrypt the encrypted message
std::vector<uint8_t> decrypted(length);
aes.decrypt(ciphertext.data(), decrypted.data(), length);

// Use decrypted.data() for the decrypted output
```
## Building

To build `sm-aes`, you'll need a C++ compiler that supports C++11 or later, as well as support for SIMD instructions.

Using g++
```bash
g++ src/FastAES.cpp src/main.cpp -o bin/sm-aes.exe -maes -msse4 -m64 -O3 -std=c++11
```
Using provided makefile with make
```bash
make
```

## Benchmark
The benchmark results were obtained by measuring the performance of AES encryption and decryption operations for different data sizes. The testing was conducted as follows:

### Procedure

1. **Test Sizes**: Four different data sizes were used for the benchmarks: 512 MB, 1 GB, 2 GB (randomly generated).

2. **Test Runs**: For each data size, the operations were repeated across multiple runs (10 in this case).

3. **Operations**:
   - **Encryption**
   - **Decryption**

4. **Measurement**:
   - **Time**: The time taken to complete the encryption and decryption operations was measured using high-resolution timers. 
   - **Encryption / Decryption rates**: The average rate was calculated as the data size divided by the average time, giving the throughput in MB/s.

For benchmark build instructions and exhaustive results, see : [Benchmark.md](Benchmark.md)

### Results

The results for each data size are summarized in the table below:

| Size    | Operation   | Average Time  | Average Rate |
|---------|-------------|---------------|--------------|
| **512 MB** | Encryption  | 0.0946 seconds | 5995.53 MB/s |
|         | Decryption  | 0.1013 seconds | 5608.9 MB/s  |
| **1024 MB** | Encryption  | 0.1864 seconds | 6117.79 MB/s |
|         | Decryption  | 0.1902 seconds | 6022.73 MB/s |
| **2048 MB** | Encryption  | 0.3620 seconds | 6152.65 MB/s |
|         | Decryption  | 0.6100 seconds | 4474.26 MB/s |

### Benchmark Environment

The benchmarks were conducted on the following hardware:

- **CPU:** Intel(R) Core(TM) i7-4810MQ @ 2.80GHz, 4 cores, 8 threads
- **RAM:** 16 GB DDR3 @ 1600MHz
- **Operating System:** Windows 10 Professional (64-bit)
- **Compiler:** g++ (GCC) 6.1.0

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE.md) file for details.