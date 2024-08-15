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
Build the benchmark with
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


## Encryption Performance

### 512 MB

| Run | Mean Time (s) | Mean Rate (MB/s) |
|-----|---------------|------------------|
| 1   | 0.207556      | 2500.5           |
| 2   | 0.124401      | 4465.53          |
| 3   | 0.0998993     | 5591.14          |
| 4   | 0.0943002     | 6002.41          |
| 5   | 0.0988986     | 5797.6           |
| 6   | 0.0988001     | 5778.46          |
| 7   | 0.0967396     | 5876.58          |
| 8   | 0.122201      | 5095.16          |
| 9   | 0.0973001     | 5787.21          |
| 10  | 0.0946003     | 5995.53          |

**Average Encryption Time**: 0.0946003 seconds  
**Average Encryption Rate**: 5995.53 MB/s

---

### 1024 MB

| Run | Mean Time (s) | Mean Rate (MB/s) |
|-----|---------------|------------------|
| 1   | 0.1872        | 5471.08          |
| 2   | 0.1879        | 6003.46          |
| 3   | 0.335999      | 4003.75          |
| 4   | 0.1852        | 5940.53          |
| 5   | 0.187601      | 6058.63          |
| 6   | 0.339295      | 3949.06          |
| 7   | 0.1859        | 5910.19          |
| 8   | 0.2084        | 5776.09          |
| 9   | 0.1831        | 6177.22          |
| 10  | 0.1864        | 6117.79          |

**Average Encryption Time**: 0.1864 seconds  
**Average Encryption Rate**: 6117.79 MB/s

---

### 2048 MB

| Run | Mean Time (s) | Mean Rate (MB/s) |
|-----|---------------|------------------|
| 1   | 0.495         | 4590.01          |
| 2   | 0.367899      | 6029.87          |
| 3   | 0.5545        | 4777.29          |
| 4   | 0.455892      | 5466.97          |
| 5   | 0.363236      | 6187.51          |
| 6   | 0.614101      | 4378.97          |
| 7   | 0.363654      | 6072.81          |
| 8   | 0.3613        | 6278.41          |
| 9   | 0.540699      | 4906.9           |
| 10  | 0.362         | 6152.65          |

**Average Encryption Time**: 0.362 seconds  
**Average Encryption Rate**: 6152.65 MB/s


## Decryption Performance

### 512 MB

| Run | Mean Time (s) | Mean Rate (MB/s) |
|-----|---------------|------------------|
| 1   | 0.1851        | 3104.08          |
| 2   | 0.126323      | 4550.55          |
| 3   | 0.1058        | 5337.26          |
| 4   | 0.0990002     | 5728.98          |
| 5   | 0.2074        | 3055.9           |
| 6   | 0.1001        | 5434.81          |
| 7   | 0.0972132     | 5827.74          |
| 8   | 0.222699      | 2896.2           |
| 9   | 0.0997002     | 5467.53          |
| 10  | 0.1013        | 5608.9           |

**Average Decryption Time**: 0.1013 seconds  
**Average Decryption Rate**: 5608.9 MB/s

---

### 1024 MB

| Run | Mean Time (s) | Mean Rate (MB/s) |
|-----|---------------|------------------|
| 1   | 0.1888        | 5430.73          |
| 2   | 0.392591      | 3267.6           |
| 3   | 0.190201      | 5720.44          |
| 4   | 0.1887        | 6006.72          |
| 5   | 0.342002      | 4084.23          |
| 6   | 0.1833        | 6001.08          |
| 7   | 0.190298      | 5992.72          |
| 8   | 0.321199      | 4348.01          |
| 9   | 0.1861        | 5955.02          |
| 10  | 0.190212      | 6022.73          |

**Average Decryption Time**: 0.190212 seconds  
**Average Decryption Rate**: 6022.73 MB/s

---

### 2048 MB

| Run | Mean Time (s) | Mean Rate (MB/s) |
|-----|---------------|------------------|
| 1   | 0.3592        | 5704.84          |
| 2   | 0.6157        | 4390.85          |
| 3   | 0.363807      | 6075.19          |
| 4   | 0.359597      | 6306.98          |
| 5   | 0.612051      | 4447.74          |
| 6   | 0.3666        | 6035.44          |
| 7   | 0.46239       | 5485.06          |
| 8   | 0.469406      | 5379.25          |
| 9   | 0.3623        | 6194.06          |
| 10  | 0.609958      | 4474.26          |

**Average Decryption Time**: 0.609958 seconds  
**Average Decryption Rate**: 4474.26 MB/s