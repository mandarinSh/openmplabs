#include <omp.h>
#include <cmath>
#include <iostream>
const unsigned int N = 1000;

int main()
{
    double b[N] = { 0 };
    double s = 0;
    #pragma omp parallel for schedule(static) reduction(+:s)
    for (size_t i = 0; i < N; ++i)
    {
        b[i] = i * sqrt(i);
        s += b[i];
    }
    std::cout.precision(15);
    std::cout << s << '\n';
    return 0;
}