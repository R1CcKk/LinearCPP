# Performance Analysis

The following benchmarks were conducted on an **Apple M2 chip** with **8GB of LPDDR5 Unified Memory**. All tests were compiled using `clang++` with the `-O3` optimization flag to ensure maximum executable efficiency.

---

## 1. Matrix Multiplication: Classical vs. Strassen

**Description**: Measures the time required to multiply two dense square matrices.

**Key Insight**: This test highlights the transition from the Classical $O(n^3)$ approach to Strassen's $O(n^{2.807})$ algorithm. While the Classical method excels in **cache locality** due to its row-major "ikj" implementation, Strassen becomes significantly more efficient as the matrix size increases by reducing the total number of arithmetic operations.

* **Break-even Point**: For small matrices (up to 128x128), the Classical method is highly competitive because it lacks the memory allocation overhead of Strassen's recursive steps.
* **Scalability**: At 2048x2048, Strassen's algorithm is approximately **41% faster** than the Classical method.
* **Complexity Check**: Doubling the size for the Classical method results in an ~8x time increase, while Strassen shows a ~7.4x increase, aligning with their respective theoretical complexities.

| Dimensions | Classical (Avg) | Strassen (Avg) | Improvement |
| :--- | :--- | :--- | :--- |
| 512 x 512 | 19.0 ms | 13.8 ms | ~27% |
| 1024 x 1024 | 154 ms | 101 ms | ~34% |
| 2048 x 2048 | 1279 ms | 753 ms | **~41%** |

---

## 2. Linear System Solver (LU Decomposition)

**Description**: Measures the total time to solve $Ax = b$ using **LU Decomposition with Partial Pivoting** followed by forward and backward substitution.

**Key Insight**: Despite having a cubic complexity $O(n^3)$, the LU Solver remains the most efficient operation in the library for solving systems of equations. It requires fewer floating-point operations (FLOPs) than a full matrix-matrix multiplication of the same size.

* **Numerical Stability**: The implementation of partial pivoting ensures that the solver handles various matrix distributions without losing precision or encountering singular matrix errors.
* **Performance**: The solver maintains excellent performance even at 2048x2048, completing in under 600ms.

| Dimensions | Execution Time (Avg) | Iterations |
| :--- | :--- | :--- |
| 512 x 512 | 9.67 ms | 73 |
| 1024 x 1024 | 73.7 ms | 9 |
| 2048 x 2048 | 594.0 ms | 1 |

---

## Technical Conclusions

1. **Algorithm Selection**: For matrices smaller than 128x128, the **Classical (ikj)** approach is preferred due to its simplicity and low memory overhead.
2. **Strassen's Threshold**: Beyond 512x512, **Strassen's Algorithm** is essential for high-performance applications where computational time is the primary bottleneck.
3. **Memory Trade-off**: Strassen provides speed at the cost of higher peak memory usage due to the allocation of seven temporary sub-matrices ($M_1$ through $M_7$) at each recursion level.
