#include "utils.h"
#include <iostream>

void initializeMatrix(float **matrix, const unsigned int n) 
{
    for (size_t i = 0; i != n; ++i) 
    {
        for (size_t j = 0; j != n; ++j) 
        {
            std::cin >> matrix[i][j];
        }
    }
}

void printMatrix(float **matrix, const unsigned int n) 
{
    for (size_t i = 0; i != n; ++i) 
    {
        for (size_t j = 0; j != n; ++j) 
        {
            std::cout.precision(3);
            std::cout << *(*(matrix + i) + j) << ' ';
        }
        std::cout << '\n';
    }
}

void deleteMatrix(float **matrix, const unsigned int rows) 
{
    for (size_t i = 0; i != rows; ++i) 
    {
        delete[] matrix[i];
    }
    delete[] matrix;
}
