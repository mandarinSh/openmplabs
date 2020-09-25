#include "calculations.h"
#include "utils.h"
#include <omp.h>
#include <cstring>
#include <utility>
#include <limits>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

float determinant(float **matrix, const unsigned int n, const unsigned int threads_amount) 
{
    if (n == 1) 
    {
        return matrix[0][0];
    }
    if (n == 2) 
    {
        return ((matrix[0][0] * matrix[1][1]) - (matrix[1][0] * matrix[0][1]));
    }
    float **matrix_copy = new float *[n];
    for (size_t i = 0; i != n; ++i) 
    {
        matrix_copy[i] = new float[n];
    }
    omp_set_num_threads(threads_amount);
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < n; ++i) 
    {
        std::memcpy(matrix_copy[i], matrix[i], sizeof(matrix[0][0]) * n);
    }
    for (size_t step = 0; step != n - 1; ++step)
    {
        #pragma omp parallel for schedule(static)
        for (size_t row = step + 1; row < n; ++row)
        {
            const float coeff = -matrix_copy[row][step] / matrix_copy[step][step];
            for (size_t col = step; col != n; ++col) 
            {
                matrix_copy[row][col] += matrix_copy[step][col] * coeff;
            }
        }
    }
    float det = 1.0f;
    #pragma omp parallel for schedule(static) reduction(*:det)
    for (size_t i = 0; i < n; ++i) 
    {
        det *= matrix_copy[i][i];
    }
    deleteMatrix(matrix_copy, n);
    return det;
}

void inverseMatrix(float **matrix, const unsigned int n, const unsigned int threads_amount) 
{
    float **augmented_matrix = new float *[n];
    for (size_t i = 0; i != n; ++i) 
    {
        augmented_matrix[i] = new float[2 * n];
    }
    omp_set_num_threads(threads_amount);
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < n; ++i) 
    {
        std::memcpy(augmented_matrix[i], matrix[i], sizeof(matrix[0][0]) * n);
    }
    for (size_t i = 0; i != n; ++i)
    {
        for (size_t j = n; j != 2 * n; ++j) 
        {
            augmented_matrix[i][j] = (j == (n + i)) ? 1.0f : 0.0f;
        }
    }
    for (size_t i = n - 1; i != 0; --i) 
    {
        if (augmented_matrix[i - 1][0] < matrix[i][0]) 
        {
            std::swap(augmented_matrix[i], augmented_matrix[i - 1]);
        }
    }
    for (size_t i = 0; i != n; ++i)
    {
        #pragma omp parallel for schedule(static)
        for (size_t j = 0; j < n; ++j)
        {
            if (i != j)
            {
                const float temp = augmented_matrix[j][i] / augmented_matrix[i][i];
                for (size_t k = 0; k != 2 * n; ++k)
                {
                    augmented_matrix[j][k] -= augmented_matrix[i][k] * temp;
                }
            }
        }
    }
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < n; ++i) 
    {
        const float scale = augmented_matrix[i][i];
        for (size_t j = 0; j != 2 * n; ++j) 
        {
            augmented_matrix[i][j] /= scale;
        }
    }
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < n; ++i) 
    {
        std::memcpy(matrix[i], augmented_matrix[i] + n, sizeof(matrix[0][0]) * n);
    }
    deleteMatrix(augmented_matrix, n);
}

void launchInteraction(const unsigned int threads_amount) 
{
    std::cout << "Input size of the matrix: ";
    unsigned int n = 0;
    std::cin >> n;
    if (n == 0) 
    {
        throw std::invalid_argument("Incorrect matrix size\n");
    }
    float **matrix = new float *[n];
    for (size_t i = 0; i != n; ++i) 
    {
        matrix[i] = new float[n];
    }

    std::cout << "\nFill the matrix typing value by value using Space or Enter\n";
    initializeMatrix(matrix, n);
    std::cout << "\nMatrix:\n";
    printMatrix(matrix, n);

    const float det_value = determinant(matrix, n, threads_amount);
    if (std::abs(det_value) > std::numeric_limits<float>::epsilon()) 
    {
        std::cout << "The determinant is " << det_value << '\n';
        inverseMatrix(matrix, n, threads_amount);
        std::cout << "\nInverse matrix:\n";
        printMatrix(matrix, n);
    }
    else 
    {
        std::cout << "The determinant is 0. This is a singular matrix. Singular matrix is not invertible.\n";
    }
    deleteMatrix(matrix, n);
}
