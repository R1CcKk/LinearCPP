# LinearCPP - High-Performance C++ Linear Algebra Library

## Project Objective
The goal of this project is the development of a high-performance library for matrix computation and numerical analysis. It marks a transition from manual, C-style memory management to a modern, Object-Oriented C++ architecture, specifically optimized for hardware interaction and complex mathematical operations.

## Architectural Design: Why This Project?
Unlike standard matrix implementations that rely on arrays of pointers (e.g., `double**`), this library utilizes **contiguous 1D memory allocation**.

* **Cache Locality:** By storing elements in a single block of RAM, the library optimizes CPU cache utilization. When the CPU fetches a specific element, it pre-loads a "Cache Line" (typically 64 bytes) into the L1 cache. Because our data is contiguous, subsequent elements are already available in the fastest memory tier, drastically reducing latency.
* **Reduced Cache Misses:** Traditional pointer-based matrices often result in fragmented memory where each row is stored in a different location. Navigating between rows in such systems triggers a "Cache Miss," forcing the CPU to fetch data from the RAM—a process 100 to 200 times slower than a cache hit.
* **Efficiency:** The design minimizes dynamic heap allocations and provides a mathematically intuitive interface through operator overloading.

---

## Development Roadmap

### Phase 1: Memory Management and Container Design
The core of the library is the `Matrix<T>` template class, built upon `std::vector` for automatic memory management.
* **Implementation:** Developed constructors, the `operator()` for 2D-to-1D coordinate mapping, and file I/O utilities.
* **Optimization:** Data is loaded from external files rather than standard input to facilitate large-scale testing and automation.

### Phase 2: Computational Optimization (Strassen’s Algorithm)
To surpass the standard $O(n^3)$ complexity of nested-loop multiplication, I implemented **Strassen’s Algorithm**, which achieves an approximate complexity of $O(n^{2.807})$.

* **Divide and Conquer:** The algorithm recursively partitions matrices into four sub-quadrants. By reducing the number of required multiplications from 8 to 7 per recursive step, it significantly improves performance for large datasets.
* **Hybrid Approach:** Since recursion introduces overhead and temporary memory allocations, the system utilizes a **threshold (set at 64)**. Once sub-matrices reach this size, the library switches to a cache-optimized classical multiplication.
* **Padding Logic:** Strassen’s algorithm requires square matrices with dimensions as powers of two. I implemented helper functions for bitwise power-of-two calculations and zero-padding to ensure compatibility without mutating the original input data.



### Phase 3: LU Decomposition
For solving linear systems and calculating determinants, I implemented **LU Decomposition with Partial Pivoting** ($PA = LU$).

* **Numerical Stability:** To prevent division by zero and minimize rounding errors, the algorithm performs row swapping (pivoting) by selecting the element with the largest absolute value in the current column.
* **Permutation Tracking:** A permutation vector and a sign-toggle variable track row swaps, which is essential for preserving the correctness of the result and calculating the matrix determinant.
* **Memory Efficiency:** The decomposition is performed "in-place," where the $U$ matrix occupies the upper triangle and the $L$ matrix (with an implicit unit diagonal) occupies the lower triangle.



### Phase 4: Linear System Solver
Once the $LU$ factors are obtained, the original system $Ax = b$ is transformed into $LUx = Pb$. The solver defines an intermediate vector $y$ such that $Ux = y$.

1.  **Forward Substitution:** Solves $Ly = Pb$. Given that $L$ is a unit lower triangular matrix, the system is solved starting from $y_0$ and proceeding downwards.
2.  **Backward Substitution:** Solves $Ux = y$. Since $U$ is upper triangular, the solution is obtained by proceeding from the last variable $x_n$ upwards, dividing by the pivot $U_{ii}$ at each step.

### Phase 5: Error Analysis and Validation
The final phase focuses on the reliability of the results.
* **Precision:** Given the inherent limitations of floating-point arithmetic (`double`), the library includes verification steps to assess numerical stability.
* **Singularity Check:** The system identifies singular matrices (determinant = 0) by evaluating pivots against a near-zero epsilon value, throwing descriptive runtime errors when a solution is impossible.

---

## Quick Start

### Prerequisites
* C++11 compatible compiler (GCC, Clang, or MSVC)

### Project Structure
```text
.
├── include/           # Header files (.hpp)
├── examples/          # Demonstration and test files
└── data/              # Sample matrix files for I/O

# To compile the benchmark:
g++ -O3 -std=c++17 benchmarks/bench_matrix.cpp MatrixLibrary/*.cpp -I./MatrixLibrary -lbenchmark -lpthread -o matrix_bench

# To run:
./matrix_bench