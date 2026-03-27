# LinearCPP - High-Performance C++ Linear Algebra Library

## Project Objective

The goal of this project is the development of a high-performance library for matrix computation and numerical analysis. It marks a transition from manual, C-style memory management to a modern, Object-Oriented C++ architecture, specifically optimized for hardware interaction and complex mathematical operations.

## Architectural Design: Why This Project?

Unlike standard matrix implementations that rely on arrays of pointers (e.g., `double**`), this library utilizes **contiguous 1D memory allocation** via `std::vector<T>`.

* **Cache Locality:** By storing elements in a single block of RAM, the library optimizes CPU cache utilization (L1/L2). When the CPU fetches a specific element, it pre-loads a "Cache Line" into the cache. Because data is contiguous, subsequent elements are already available in the fastest memory tier.
* **Reduced Cache Misses:** Traditional pointer-based matrices often result in fragmented memory where each row is stored in a different location. Navigating between rows in such systems triggers a "Cache Miss," forcing the CPU to fetch data from the RAM.
* **Efficiency:** The design minimizes dynamic heap allocations and provides a mathematically intuitive interface through operator overloading (`+`, `-`, `*`).

---

## Development Roadmap

### Phase 1: Memory Management and Container Design

The core of the library is the `Matrix<T>` template class.

* **Implementation:** Developed constructors, the `operator()` for 2D-to-1D coordinate mapping, and file I/O utilities.
* **Optimization:** Data is loaded from external files rather than standard input to facilitate large-scale testing and automation.

### Phase 2: Computational Optimization (Strassen’s Algorithm)

To surpass the standard $O(n^3)$ complexity of nested-loop multiplication, I implemented **Strassen’s Algorithm**, achieving an approximate complexity of $O(n^{2.807})$.

* **Divide and Conquer:** The algorithm recursively partitions matrices into four sub-quadrants, reducing the number of required multiplications from 8 to 7 per recursive step.
* **Hybrid Approach:** Since recursion introduces overhead, the system utilizes a **threshold (set at 64)**. Once sub-matrices reach this size, the library switches to a cache-optimized classical multiplication.
* **Padding Logic:** Strassen’s algorithm requires square matrices with dimensions as powers of two. I implemented helper functions for bitwise power-of-two calculations and zero-padding.

### Phase 3: LU Decomposition

For solving linear systems and calculating determinants, I implemented **LU Decomposition with Partial Pivoting** ($PA = LU$).

* **Numerical Stability:** To prevent division by zero and minimize rounding errors, the algorithm performs row swapping (pivoting) by selecting the element with the largest absolute value in the current column.
* **Permutation Tracking:** A permutation vector tracks row swaps, which is essential for preserving the correctness of the result and calculating the matrix determinant.
* **Memory Efficiency:** The decomposition is performed packed into a single Matrix object: the upper triangle represents the $U$ matrix and the lower triangle represents the $L$ matrix.

### Phase 4: Linear System Solver

Once the $LU$ factors are obtained, the system $Ax = b$ is solved in two steps:

1. **Forward Substitution:** Solves $Ly = Pb$. Given that $L$ is a unit lower triangular matrix, the system is solved starting from $y_0$ and proceeding downwards.
2. **Backward Substitution:** Solves $Ux = y$. Since $U$ is upper triangular, the solution is obtained by proceeding from the last variable $x_n$ upwards, dividing by the pivot $U_{ii}$ at each step.

---

## Performance Analysis

The following benchmarks were conducted on an **Apple M2 chip** with **8GB of LPDDR5 Unified Memory**. Tests were compiled with `clang++` using the `-O3` flag.

| Dimension | Classical (Avg) | Strassen (Avg) | Improvement |
| :--- | :--- | :--- | :--- |
| 512 x 512 | 19.0 ms | 13.8 ms | ~27% |
| 1024 x 1024 | 154 ms | 101 ms | ~34% |
| 2048 x 2048 | 1279 ms | 753 ms | **~41%** |

---

## Quick Start

### Prerequisites

* C++17 compatible compiler (GCC, Clang)
* CMake (>= 3.10)
* Google Benchmark library
* Eigen3 (included in `externalEigen/`)

### Build and Run

```zsh
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
./matrix_bench
```
