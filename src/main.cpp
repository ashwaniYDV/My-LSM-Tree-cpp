#include <iostream>
#include <sstream>
#include <string>
#include "KeyValueStore.h"

bool processCommand(const std::string &command, KeyValueStore &kvStore)
{
    std::istringstream iss(command);
    std::string operation, key, value;

    // Extract the operation
    iss >> operation;
    std::transform(operation.begin(), operation.end(), operation.begin(), ::toupper);

    if (operation == "GET")
    {
        iss >> key;
        std::cout << kvStore.get(key).value << std::endl;
    }
    else if (operation == "SET")
    {
        iss >> key;
        std::getline(iss >> std::ws, value);
        kvStore.set(key, value);
        std::cout << "OK" << std::endl;
    }
    else if (operation == "DEL")
    {
        iss >> key;
        kvStore.del(key);
        std::cout << "OK" << std::endl;
    }
    else if (operation == "EXIT")
    {
        return false;
    }
    else
    {
        std::cout << "Invalid command" << std::endl;
    }
    return true;
}

int main()
{
    KeyValueStore kvStore;

    std::string command;

    while (true)
    {
        std::cout << ">";
        std::getline(std::cin, command);
        if (!processCommand(command, kvStore)) {
            break;
        }
    }
    return 0;
}
