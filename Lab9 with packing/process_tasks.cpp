#include "process_tasks.h"
#include "split_encrypt.h"
#include <mpi.h>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

void packAndSend(const std::string &message, const int dest, const int tag) 
{
    int outbuf_size = 0;
    MPI_Pack_size(message.length(), MPI_CHAR, MPI_COMM_WORLD, &outbuf_size);
    std::vector<char> outbuf(outbuf_size);
    MPI_Send(&outbuf_size, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
    if (outbuf_size != 0) 
    {
        int position = 0;
        MPI_Pack(message.data(), message.length(), MPI_CHAR, outbuf.data(), outbuf_size, &position, MPI_COMM_WORLD);
        MPI_Send(outbuf.data(), outbuf_size, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    }
}

void receiveAndUnpack(std::string &message, const int src, const int tag) 
{
    MPI_Status status;
    int buf_size = 0;
    MPI_Recv(&buf_size, 1, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
    if (buf_size != 0) 
    {
        std::vector<char> inbuf(static_cast<size_t>(buf_size));
        MPI_Recv(inbuf.data(), inbuf.size(), MPI_CHAR, src, tag, MPI_COMM_WORLD, &status);
        int position = 0;
        std::vector<char> outbuf(static_cast<size_t>(buf_size));
        MPI_Unpack(inbuf.data(), inbuf.size(), &position, outbuf.data(), outbuf.size(), MPI_CHAR, MPI_COMM_WORLD);
        message.assign(outbuf.begin(), outbuf.end());
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
        packAndSend(splitted_message[key[1][i + 1]], i + 1, i + 1);
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
    receiveAndUnpack(message, 0, rank);
    MPI_Status status;
    unsigned int shift = 1;
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
