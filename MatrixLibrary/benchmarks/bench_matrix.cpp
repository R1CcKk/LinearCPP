#include <benchmark/benchmark.h>
#include <vector>
#include <random>

#include "Matrix.hpp"
#include "LinearSolver.hpp"
#include "Product.hpp"

/**
 * @brief Benchmark for Classical Matrix Multiplication (ikj).
 * We call matrixMultiply directly to bypass the threshold logic in operator*.
 */
static void BM_ClassicalProduct(benchmark::State &state)
{
    int n = state.range(0);

    Matrix<double> A(n, n), B(n, n);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            A(i, j) = (double)rand() / RAND_MAX;
            B(i, j) = (double)rand() / RAND_MAX;
        }
    }

    for (auto _ : state)
    {
        Matrix<double> C = matrixMultiply(A, B);
        benchmark::DoNotOptimize(C);
    }
}

/**
 * @brief Benchmark for Strassen Matrix Multiplication.
 * This will show the overhead of recursion and sub-matrix allocations.
 */
static void BM_StrassenProduct(benchmark::State &state)
{
    int n = state.range(0);

    Matrix<double> A(n, n), B(n, n);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            A(i, j) = (double)rand() / RAND_MAX;
            B(i, j) = (double)rand() / RAND_MAX;
        }
    }

    for (auto _ : state)
    {
        Matrix<double> C = strassenMultiply(A, B);
        benchmark::DoNotOptimize(C);
    }
}

/**
 * @brief Benchmark for the Linear System Solver (Ax = b).
 * Measures the performance of LU Decomposition + Forward/Backward substitution.
 */
static void BM_LUSolver(benchmark::State &state)
{
    int n = state.range(0);

    Matrix<double> A(n, n);
    std::vector<double> b(n);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            A(i, j) = 1.0 + (double)rand() / RAND_MAX;
        }
        b[i] = 1.0 + (double)rand() / RAND_MAX;
        A(i, i) += n; // Diagonally dominant to ensure it's not singular
    }

    for (auto _ : state)
    {
        auto lu_result = decomposeLU(A); 
        auto x = solve(lu_result, b);    
        benchmark::DoNotOptimize(x);
    }
}


BENCHMARK(BM_ClassicalProduct)->RangeMultiplier(2)->Range(64, 2048)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_StrassenProduct)->RangeMultiplier(2)->Range(64, 2048)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_LUSolver)->RangeMultiplier(2)->Range(64, 2048)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();