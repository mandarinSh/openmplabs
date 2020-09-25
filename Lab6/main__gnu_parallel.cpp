#include "omp.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <parallel/algorithm>

int main(int argc, char *argv[])
{
    try 
    {
        if ((argc != 4) 
        {
            throw std::invalid_argument("Incorrect amount of arguments");
        }
    } 
    catch (const std::invalid_argument &ex) 
    {
        std::cerr << ex.what() << '\n';
        return 1;
    }
    const std::string consequence = argv[1];
    std::cout << "Consequence: " << consequence << '\n';
    const char symbol = argv[2][0];
    std::cout << "Symbol: " << symbol << '\n';
    const unsigned int amount = __gnu_parallel::count(consequence.begin(), consequence.end(), symbol);
    std::cout << "Amount of occurrences: " << amount << '\n';
    return 0;
}
