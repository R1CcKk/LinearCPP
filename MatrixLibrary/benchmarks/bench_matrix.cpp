#include <benchmark/benchmark.h>
#include "Matrix.hpp"
#include "LinearSolver.hpp"
#include <vector>
#include <random>

/**
 * @brief Benchmark for Matrix Multiplication (A * B).
 * Evaluates the performance of the overloaded * operator and cache efficiency.
 */
void BM_MatrixMultiplication(benchmark::State& state){

    int n = state.range(0);

    Matrix<double> A(n,n), B(n,n);

    for (int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j){
            A(i, j) = (double)rand() / (RAND_MAX / 10.0);
        }
    }
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            B(i, j) = (double)rand() / (RAND_MAX / 10.0);
        }
    }

    for (auto _ : state)
    {
        Matrix<double> C = A*B;
        benchmark::DoNotOptimize(C); 
    }
}

/**
 * @brief Benchmark for Linear System Solver (Ax = b).
 * Measures the combined execution time of LU Decomposition and Substitution.
 */
void BM_LinearSolver(benchmark::State &state)
{
    int n = state.range(0);

    Matrix<double> A(n, n);
    std::vector<double> b(n);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            //+1 to avoid zero entries which could lead to singular matrices
            A(i, j) = 1.0 + (double)rand() / (RAND_MAX / 9.0);
        }
        b[i] = 1.0 + (double)rand() / (RAND_MAX / 9.0);
        //I need to to ensure that the matrix is not singular, so I add a value to the diagonal elements
        A(i, i) += n;
    }

    for (auto _ : state)
    {
        auto lu_result = decomposeLU(A);
        auto x = solve(lu_result, b);

        benchmark::DoNotOptimize(x);
    }
}

BENCHMARK(BM_LinearSolver)->Arg(500)->Arg(750)->Arg(1000)->Arg(2000)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_MatrixMultiplication)->Arg(500)->Arg(750)->Arg(1000)->Arg(2000)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
