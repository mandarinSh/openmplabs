#ifndef CALCULATIONS_H
#define CALCULATIONS_H

float determinant(float **matrix, const unsigned int n, const unsigned int threads_amount);

void inverseMatrix(float **matrix, const unsigned int n, const unsigned int threads_amount);

void launchInteraction(const unsigned int threads_amount);

#endif
