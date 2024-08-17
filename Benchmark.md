# Benchmark Results

The benchmark results were obtained by measuring the performance of AES encryption and decryption operations for different data sizes. The testing was conducted as follows:

### Procedure

1. **Test Sizes**: Four different data sizes were used for the benchmarks: 512 MB, 1 GB, 2 GB (randomly generated).

2. **Test Runs**: For each data size, the operations were repeated across multiple runs (10 in this case) to ensure accuracy and reliability of the results.

3. **Operations**:
   - **Encryption**: The AES encryption operation was performed on randomly generated data.
   - **Decryption**: The AES decryption operation was performed on the encrypted data obtained from the encryption step.

4. **Measurement**:
   - **Time**: The time taken to complete the encryption and decryption operations was measured using high-resolution timers. 
   - **Rate**: The average rate was calculated as the data size divided by the average time, giving the throughput in MB/s.

### Build
Build the MT-AES benchmark with
```sh
make benchmark
```
Or  
```sh
g++ src/FastAES.cpp src/benchmark.cpp -o bin/benchmark.exe -maes -msse4 -m64 -O3 -std=c++11
```
And then run with :
```sh
bin/benchmark.exe
```

For Building Openssl benchmark, use:
```sh
g++ -o benchmark_openssl src/benchmark_openssl.cpp -lssl -lcrypto -m64 -O3 -std=c++11
```
Make sure ou have Openssl installed and well configured.

## MT-AES Performance

### 512 MB

| Run | Encryption Time (s) | Encryption Rate (MB/s) | Decryption Time (s) | Decryption Rate (MB/s) |
|-----|----------------------|------------------------|----------------------|------------------------|
| 1   | 0.207556             | 2500.5                 | 0.1851               | 3104.08                |
| 2   | 0.124401             | 4465.53                | 0.126323             | 4550.55                |
| 3   | 0.0998993            | 5591.14                | 0.1058               | 5337.26                |
| 4   | 0.0943002            | 6002.41                | 0.0990002            | 5728.98                |
| 5   | 0.0988986            | 5797.60                | 0.2074               | 3055.90                |
| 6   | 0.0988001            | 5778.46                | 0.1001               | 5434.81                |
| 7   | 0.0967396            | 5876.58                | 0.0972132            | 5827.74                |
| 8   | 0.122201             | 5095.16                | 0.222699             | 2896.20                |
| 9   | 0.0973001            | 5787.21                | 0.0997002            | 5467.53                |
| 10  | 0.0946003            | 5995.53                | 0.1013               | 5608.90                |

**Average Encryption Time**: 0.0946003 seconds  
**Average Encryption Rate**: 5995.53 MB/s  
**Average Decryption Time**: 0.1013 seconds  
**Average Decryption Rate**: 5608.90 MB/s

---

### 1024 MB

| Run | Encryption Time (s) | Encryption Rate (MB/s) | Decryption Time (s) | Decryption Rate (MB/s) |
|-----|----------------------|------------------------|----------------------|------------------------|
| 1   | 0.1872               | 5471.08                | 0.1888               | 5430.73                |
| 2   | 0.1879               | 6003.46                | 0.392591             | 3267.60                |
| 3   | 0.335999             | 4003.75                | 0.190201             | 5720.44                |
| 4   | 0.1852               | 5940.53                | 0.1887               | 6006.72                |
| 5   | 0.187601             | 6058.63                | 0.342002             | 4084.23                |
| 6   | 0.339295             | 3949.06                | 0.1833               | 6001.08                |
| 7   | 0.1859               | 5910.19                | 0.190298             | 5992.72                |
| 8   | 0.2084               | 5776.09                | 0.321199             | 4348.01                |
| 9   | 0.1831               | 6177.22                | 0.1861               | 5955.02                |
| 10  | 0.1864               | 6117.79                | 0.190212             | 6022.73                |

**Average Encryption Time**: 0.1864 seconds  
**Average Encryption Rate**: 6117.79 MB/s  
**Average Decryption Time**: 0.190212 seconds  
**Average Decryption Rate**: 6022.73 MB/s

---

### 2048 MB

| Run | Encryption Time (s) | Encryption Rate (MB/s) | Decryption Time (s) | Decryption Rate (MB/s) |
|-----|----------------------|------------------------|----------------------|------------------------|
| 1   | 0.495                | 4590.01                | 0.3592               | 5704.84                |
| 2   | 0.367899             | 6029.87                | 0.6157               | 4390.85                |
| 3   | 0.5545               | 4777.29                | 0.363807             | 6075.19                |
| 4   | 0.455892             | 5466.97                | 0.359597             | 6306.98                |
| 5   | 0.363236             | 6187.51                | 0.612051             | 4447.74                |
| 6   | 0.614101             | 4378.97                | 0.3666               | 6035.44                |
| 7   | 0.363654             | 6072.81                | 0.46239              | 5485.06                |
| 8   | 0.3613               | 6278.41                | 0.469406             | 5379.25                |
| 9   | 0.540699             | 4906.90                | 0.3623               | 6194.06                |
| 10  | 0.362                | 6152.65                | 0.609958             | 4474.26                |

**Average Encryption Time**: 0.362 seconds  
**Average Encryption Rate**: 6152.65 MB/s  
**Average Decryption Time**: 0.609958 seconds  
**Average Decryption Rate**: 4474.26 MB/s

## OpenSSL + AESNI Performance

### 512 MB

| Run | Encryption Time (s) | Encryption Rate (MB/s) | Decryption Time (s) | Decryption Rate (MB/s) |
|-----|----------------------|------------------------|----------------------|------------------------|
| 1   | 0.35389              | 2009.76                | 0.153274             | 3913.43                |
| 2   | 0.119365             | 4494.00                | 0.120173             | 4653.11                |
| 3   | 0.12841              | 4441.92                | 0.130688             | 4405.94                |
| 4   | 0.239292             | 3516.65                | 0.118831             | 4760.02                |
| 5   | 0.123562             | 4502.53                | 0.328792             | 2631.05                |
| 6   | 0.126992             | 4482.62                | 0.116825             | 4646.35                |
| 7   | 0.135199             | 4247.02                | 0.124637             | 4580.64                |
| 8   | 0.240421             | 3174.87                | 0.282670             | 3126.04                |
| 9   | 0.133214             | 4177.60                | 0.132444             | 4244.86                |
| 10  | 0.136745             | 4172.45                | 0.125878             | 4493.89                |

**Average Encryption Time**: 0.136745 seconds  
**Average Encryption Rate**: 4172.45 MB/s  
**Average Decryption Time**: 0.125878 seconds  
**Average Decryption Rate**: 4493.89 MB/s

---

### 1024 MB

| Run | Encryption Time (s) | Encryption Rate (MB/s) | Decryption Time (s) | Decryption Rate (MB/s) |
|-----|----------------------|------------------------|----------------------|------------------------|
| 1   | 0.474335             | 3099.61                | 0.218647             | 4690.48                |
| 2   | 0.243285             | 4544.28                | 0.277334             | 4177.48                |
| 3   | 0.239264             | 4747.61                | 0.587497             | 2745.92                |
| 4   | 0.554823             | 3123.17                | 0.261606             | 4261.21                |
| 5   | 0.276432             | 4036.34                | 0.623415             | 2525.28                |
| 6   | 0.567308             | 2993.06                | 0.235231             | 4610.19                |
| 7   | 0.245503             | 4475.21                | 0.247272             | 4634.53                |
| 8   | 0.265750             | 4308.47                | 0.424425             | 4093.39                |
| 9   | 0.322175             | 4262.73                | 0.279883             | 4105.55                |
| 10  | 0.244411             | 4620.16                | 0.505460             | 3348.57                |

**Average Encryption Time**: 0.244411 seconds  
**Average Encryption Rate**: 4620.16 MB/s  
**Average Decryption Time**: 0.505460 seconds  
**Average Decryption Rate**: 3348.57 MB/s

---

### 2048 MB

| Run | Encryption Time (s) | Encryption Rate (MB/s) | Decryption Time (s) | Decryption Rate (MB/s) |
|-----|----------------------|------------------------|----------------------|------------------------|
| 1   | 0.815344             | 3386.45                | 0.523091             | 3929.81                |
| 2   | 0.578493             | 4032.10                | 0.534489             | 4245.29                |
| 3   | 0.472795             | 4747.64                | 0.836208             | 3633.12                |
| 4   | 0.485522             | 4712.28                | 0.801597             | 3765.53                |
| 5   | 0.830196             | 3719.75                | 0.806600             | 3736.91                |
| 6   | 0.817972             | 3768.42                | 0.508967             | 4408.42                |
| 7   | 0.516141             | 4430.96                | 0.530948             | 4378.04                |
| 8   | 0.483359             | 4716.09                | 0.828667             | 3696.41                |
| 9   | 0.692191             | 3887.62                | 0.827709             | 3643.16                |
| 10  | 0.811207             | 3781.40                | 0.845851             | 3479.06                |

**Average Encryption Time**: 0.811207 seconds  
**Average Encryption Rate**: 3781.40 MB/s  
**Average Decryption Time**: 0.845851 seconds  
**Average Decryption Rate**: 3479.06 MB/s