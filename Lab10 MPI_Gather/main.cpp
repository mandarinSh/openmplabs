#include <mpi.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[]) 
{
    MPI_Init(&argc, &argv);
    int rank = 0, size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::srand(static_cast<unsigned int>(std::time(nullptr) + rank));
    std::vector<int> values;
    if (rank == 0) 
    {
        values.resize(static_cast<size_t>(size));
    }
    const int value = std::rand() % 10;
    const double start_time = MPI_Wtime();
    MPI_Gather(&value, 1, MPI_INT, values.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
    const double elapsed_time = MPI_Wtime() - start_time;
    double total_time = 0.0;
    MPI_Reduce(&elapsed_time, &total_time, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) 
    {
        for (size_t i = 0; i != values.size(); ++i) 
        {
            std::cout << "Received value [" << i << "]: " << values[i] << '\n';
        }
        std::cout << "Execution time: " << total_time << '\n';
    }
    MPI_Finalize();
    return 0;
}
