#include <omp.h>
#include <string>
#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[]) 
{
    try 
    {
        if (argc != 4) 
        {
            throw std::invalid_argument("Incorrect amount of arguments");
        }
        unsigned int threads_amount = std::stoi(argv[3]);
        if (threads_amount > 4) 
        {
            threads_amount = 4;
        }
        std::cout << "Amount of threads: " << threads_amount << '\n';
        omp_set_num_threads(threads_amount);
    } 
    catch (const std::invalid_argument &ex) 
    {
        std::cerr << ex.what() << '\n';
        return 1;
    } 
    catch (const std::out_of_range &ex) 
    {
        std::cerr << ex.what() << '\n';
        return 1;
    }
    const std::string consequence = argv[1];
    std::cout << "Consequence: " << consequence << '\n';
    const char symbol = argv[2][0];
    std::cout << "Symbol: " << symbol << '\n';
    unsigned int count = 0;
    #pragma omp parallel for schedule(static) reduction(+:count)
    for (size_t i = 0; i < consequence.length(); ++i)
    {
        if (consequence[i] == symbol) 
        {
            ++count;
        }
    }
    std::cout << "Amount of occurrences: " << count << '\n';
    return 0;
}
