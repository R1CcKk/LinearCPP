#include "Matrix.hpp"
#include <vector>
#include <cmath>
#include <stdexcept>
#include<type_traits>

/**
 * @brief Structure to store the results of an LU Decomposition with Partial Pivoting.
 * * To optimize memory usage, the L and U matrices are packed into a single Matrix object:
 * - The upper triangle (including the diagonal) represents the U matrix.
 * - The lower triangle (excluding the diagonal) represents the L matrix (unit lower triangular).
 */
template <typename T>
    struct LUResult {
    Matrix<T> LU; //< Packed L and U matrices.
    std::vector<int> P; //< Permutation vector tracking row swaps (pivoting).
    int toggleSign;     //< Tracks the number of swaps to determine the determinant sign.
    };

    /**
     * @brief Performs LU Decomposition with Partial Pivoting (PA = LU).
     * * This function decomposes a square matrix into a lower triangular matrix (L)
     * and an upper triangular matrix (U). Partial pivoting is used to ensure
     * numerical stability by swapping rows to bring the largest absolute value
     * to the pivot position.
     * * @note This implementation requires floating-point types (float, double) to
     * handle division and precision.
     * * @tparam T Must be a floating-point type.
     * @param A The square matrix to decompose.
     * @return An LUResult structure containing the packed LU matrix and permutation data.
     * @throws std::runtime_error If the matrix is singular (zero pivot encountered).
     */
    template <typename T>

    LUResult<T> decomposeLU(const Matrix<T>& A){
        LUResult<T> result;

        static_assert(
            std::is_floating_point<T>::value,
            "LU decomposition requires floating-point types");

        int dim = A.getRows();
        result.LU = A;
        result.P.resize(dim);
        result.toggleSign = 1;

        for(auto i = 0; i < dim; ++i){
            result.P[i] = i;
        }

        for(int i = 0; i < dim; ++i){
            int maxIndex = i;
            T maxVal = std::abs(result.LU(i,i));
            for(int j = i + 1; j < dim; ++j){
                if(std::abs(result.LU(j,i)) > maxVal){
                    maxVal = std::abs(result.LU(j,i));
                    maxIndex = j;
                }
            }
            if(maxIndex != i){
                for(int j = i; j < dim; ++j){
                    T tmp = result.LU(i,j);
                    result.LU(i,j) = result.LU(maxIndex,j);
                    result.LU(maxIndex,j) = tmp;
                }
                
                int tmp = result.P[i];
                result.P[i] = result.P[maxIndex];
                result.P[maxIndex] = tmp;
                result.toggleSign *= -1;
            }
            if (std::abs(result.LU(i, i)) < 1e-15)
            {
                throw std::runtime_error("Error: Singular matrix. Null pivot at index " + std::to_string(i));
            }
            for (auto j = i + 1; j < dim; ++j)
            {
                T mult = result.LU(j, i) / result.LU(i, i);
                result.LU(j, i) = mult;
                for (auto k = i + 1; k < dim; ++k)
                {
                    result.LU(j, k) -= mult * result.LU(i, k);
                }
            }
        }
        
        



        return result;
    }

    /**
     * @brief Solves the linear system Ax = b using a previously computed LU decomposition.
     * * The solution process consists of three steps:
     * 1. Permute the input vector b according to the pivoting performed during decomposition (Pb).
     * 2. Forward substitution to solve Ly = Pb.
     * 3. Backward substitution to solve Ux = y.
     * * @tparam T Floating-point type of the elements.
     * @param m_LU The LUResult structure containing the decomposed matrix and permutation vector.
     * @param b The right-hand side constant vector.
     * @return A vector containing the solution x.
     */
    template<typename T>
    std::vector<T> solve(const LUResult<T> &m_LU, const std::vector<T> &b){
        int dim = m_LU.LU.getRows();

        // Apply permutation to the vector b
        std::vector<T> pb(dim);
        for (auto i = 0; i < dim; ++i)
        {
            pb[i] = b[m_LU.P[i]];
        }

        // Forward Substitution (Ly = Pb)
        // L is unit lower triangular (diagonal is implicitly 1)
        std::vector<T> y(dim);
        for (auto i = 0; i < dim; ++i)
        {
            T sum = 0;
            for (auto j = 0; j < i; ++j)
            {
                sum += m_LU.LU(i, j) * y[j];
            }
            y[i] = pb[i] - sum;
        }

        // Backward Substitution (Ux = y)
        // U is upper triangular (diagonal is stored in the LU matrix)
        std::vector<T> x(dim);
        for (auto i = dim - 1; i >= 0; --i)
        {
            T sum = 0;
            for (auto j = i + 1; j < dim; ++j)
            {
                sum += m_LU.LU(i, j) * x[j];
            }
            x[i] = (y[i] - sum) / m_LU.LU(i, i);
        }
        return x;
    }