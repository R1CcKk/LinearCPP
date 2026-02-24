## Performance Analysis

The following benchmarks were conducted on an Apple M2 chip with 8GB of LPDDR5 Unified Memory. All tests were compiled using clang++ with the -O3 optimization flag.

### 1. Matrix Multiplication

Description: Measures the time to multiply two square matrices.

Key Insight: This test highlights the efficiency of the Row-Major storage. Accessing data linearly in memory maximizes L1/L2 cache hits, preventing "cache trashing."

Scaling: You can observe that doubling the size (e.g., from 500 to 1000) results in an approximately 8x increase in execution time, consistent with cubic complexity.

Dimensions,Execution Time (Avg),Iterations
500 x 500,14.13 ms,49
750 x 750,101.29 ms,49
1000 x 1000,102.47 ms,6
2000 x 2000,795.45 ms,1


### 2. Linear System Solver 

Description: Measures the resolution of Ax=b using Partial Pivoting (decomposeLU) followed by forward and backward substitution (solve).

Key Insight: Unlike simple multiplication, this algorithm involves partial pivoting and back-substitution.

Benchmark Goal: To ensure numerical stability and performance even as the system of equations grows.

Dimensions,Execution Time (Avg),Iterations
500 x 500,8.99 ms,78
750 x 750,29.70 ms,24
1000 x 1000,70.80 ms,10
2000 x 2000,572.00 ms,1