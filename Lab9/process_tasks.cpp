#include "process_tasks.h"
#include "split_encrypt.h"
#include <mpi.h>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

void sendMessage(const std::string &message, const int dest, const int tag, MPI_Comm comm) 
{
    const unsigned int length = message.length();
    MPI_Send(&length, 1, MPI_UNSIGNED, dest, tag, comm);
    if (length != 0) 
    {
        MPI_Send(message.data(), length, MPI_CHAR, dest, tag, comm);
    }
}

void receiveMessage(std::string &message, const int src, const int tag, MPI_Comm comm) 
{
    unsigned int length = 0;
    MPI_Status status;
    MPI_Recv(&length, 1, MPI_UNSIGNED, src, tag, comm, &status);
    if (length != 0) 
    {
        std::vector<char> tmp(length);
        MPI_Recv(tmp.data(), length, MPI_CHAR, src, tag, comm, &status);
        message.assign(tmp.begin(), tmp.end());
    }
}

void firstProcessTask(unsigned int units_number) 
{
    std::string message;
    std::cout << "Input message: ";
    std::getline(std::cin, message, '\n');
    unsigned int shift = 1;
    std::cout << "Input shift value: ";
    std::cin >> shift;
    std::vector<unsigned int> key[2];
    for (size_t j = 0; j != units_number; ++j) 
    {
        key[0].push_back(j);
        key[1].push_back(j); 
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(key[1].begin(), key[1].end(), g);

    std::vector<std::string> splitted_message = sptitMessageIntoParts(message, units_number);

    for (size_t i = 0; i != units_number - 1; ++i) 
    {
        sendMessage(splitted_message[key[1][i + 1]], i + 1, i + 1, MPI_COMM_WORLD);
        MPI_Send(&shift, 1, MPI_UNSIGNED, i + 1, i + 1, MPI_COMM_WORLD);
    }

    if (!message.empty()) 
    {
        message = encryptCaesarCipher(splitted_message[key[1][0]], shift);
        std::cout << "Process 0. Encrypted piece: " << message << '\n';
    }
    else 
    {
        std::cout << "Process 0. An empty message received\n";
    }
}

void otherProcessTask(const int rank) 
{
    std::string message;
    receiveMessage(message, 0, rank, MPI_COMM_WORLD);
    unsigned int shift = 1;
    MPI_Status status;
    MPI_Recv(&shift, 1, MPI_UNSIGNED, 0, rank, MPI_COMM_WORLD, &status);
    if (!message.empty()) 
    {
        message = encryptCaesarCipher(message, shift);
        std::cout << "Process " << rank << ". Encrypted piece: " << message << '\n';
    }
    else 
    {
        std::cout << "Process " << rank << ". An empty message received\n";
    }
}
