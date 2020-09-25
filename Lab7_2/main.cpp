#include "utils.h"
#include "calculations.h"
#include <string>
#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[]) 
{
    try 
    {
        if (argc != 2) 
        {
            throw std::invalid_argument("Incorrect amount of arguments\n");
        }
        const int threads_amount = std::stoi(argv[1]);
        if (threads_amount < 1) 
        {
            throw std::invalid_argument("Amount of threads can not be zero or less\n");
        }
        launchInteraction(static_cast<unsigned int>(threads_amount));
    } catch (const std::invalid_argument &arg) 
    {
        std::cerr << arg.what();
        return 1;
    } 
    catch (const std::out_of_range &ex) 
    {
        std::cerr << ex.what();
        return 1;
    }
    return 0;
}
