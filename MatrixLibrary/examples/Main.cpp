#include <iostream>
#include <vector>
#include <exception>
#include "Matrix.hpp"
#include "Helper.hpp"
#include "Product.hpp" 
#include "LinearSolver.hpp"

/**
 * Demo program for LinearCPP library.
 * This executable demonstrates:
 * 1. Loading matrices from files.
 * 2. Strassen matrix multiplication.
 * 3. LU Decomposition with partial pivoting.
 * 4. Solving linear systems Ax = b.
 * 5. Numerical verification of the results.
 */

int main(int argc, char *argv[])
{

    if (argc < 5)
    {
        std::cout << "Usage: " << argv[0] << " <fileA.txt> <fileB.txt> <filevb.txt> <output.txt>" << std::endl;
        return 1;
    }

    try
    {
        // --- 1. Matrix Loading ---
        std::cout << "--- Loading Matrices ---" << std::endl;
        Matrix<double> matA = Matrix<double>::fromFile(argv[1]);
        Matrix<double> matB = Matrix<double>::fromFile(argv[2]);

        std::vector<double> b = loadVectorFromFile<double>(argv[3]);

        std::cout << "Matrix A: " << matA.getRows() << "x" << matA.getCols() << std::endl;
        std::cout << "Matrix B: " << matB.getRows() << "x" << matB.getCols() << std::endl;
        std::cout << "Vector b: " << b.size() << "x" << "1" << std::endl;
        

        // --- 2. Multiplication (Strassen Algorithm) ---
        std::cout << "\n--- Computing Matrix Product (A * B) ---" << std::endl;
        // The operator* internally chooses between classical and Strassen
        Matrix<double> matProd = matA * matB;

        std::cout << "Product calculated. Saving to " << argv[4] << "..." << std::endl;
        matProd.toFile(argv[4]);

        // --- 3. LU Decomposition ---
        std::cout << "\n--- Performing LU Decomposition on A ---" << std::endl;
        Matrix<double> luInput = matA;
        if (luInput.getRows() != luInput.getCols())
        {
            throw std::runtime_error("LU Decomposition requires a square matrix.");
        }

        LUResult<double> luRes = decomposeLU(luInput);
        std::cout << "Packed L/U Matrix:" << std::endl;
        luRes.LU.printMatrix();

        // --- 4. Linear System Solver ---
        std::cout << "\n--- Solving System Ax = b ---" << std::endl;
        std::vector<double> x = solve(luRes, b);

        std::cout << "Solution vector x:" << std::endl;
        for (double val : x)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;

        // --- 5. Final Verification ---
        std::cout << "\n--- Verifying Result (A * x == b) ---" << std::endl;

        Matrix<double> x_mat(x, true);
        Matrix<double> b_mat(b, true);

        check(matA, x_mat, b_mat);
    }
    catch (const std::exception &e)
    {
        std::cerr << "\n[ERROR]: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}