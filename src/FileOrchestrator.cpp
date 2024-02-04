#include "FileOrchestrator.h"
#include <fstream>
#include <iostream>

FileOrchestrator::FileOrchestrator(const std::string &dataFilePath, const std::string &indexFilePath)
    : dataFilePath(dataFilePath), indexFilePath(indexFilePath)
{
    loadIndex();
}

FileOrchestrator::~FileOrchestrator()
{
    saveIndex();
}

void FileOrchestrator::saveIndex()
{
    std::ofstream file(indexFilePath, std::ios::binary | std::ios::trunc);
    if (!file.is_open())
    {
        std::cerr << "Error opening "<< indexFilePath << " for persisting index." << std::endl;
        return;
    }

    // Write the key-value pairs to the index file in binary format
    for (const auto &pair : indexTable)
    {
        int keySize = static_cast<int>(pair.first.size());
        file.write(reinterpret_cast<const char *>(&keySize), sizeof(int));
        file.write(pair.first.c_str(), keySize);

        file.write(reinterpret_cast<const char *>(&pair.second), sizeof(long long));
    }

    file.close();

    std::cout << "Index saved to " << indexFilePath << std::endl;
}

void FileOrchestrator::loadIndex()
{
    std::ifstream file(indexFilePath, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error opening " << indexFilePath << " for loading index." << std::endl;
        return;
    }

    while (true)
    {
        int keySize;
        if (!file.read(reinterpret_cast<char *>(&keySize), sizeof(int)))
            break;

        std::string key(keySize, '\0');
        if (!file.read(&key[0], keySize))
            break;

        long long value;
        if (!file.read(reinterpret_cast<char *>(&value), sizeof(long long)))
            break;

        indexTable[key] = value;
    }

    file.close();

    std::cout << "Index loaded from " << indexFilePath << std::endl;
}

void FileOrchestrator::write(const std::string &key, const std::string &value)
{
    std::ofstream file(dataFilePath, std::ios::ate | std::ios::app | std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Error opening " << dataFilePath << " for writing data." << std::endl;
        return;
    }

    long long byteOffset = file.tellp();

    indexTable[key] = byteOffset;

    int keySize = static_cast<int>(key.size());
    file.write(reinterpret_cast<const char *>(&keySize), sizeof(int));
    file.write(key.c_str(), keySize);

    int valueSize = static_cast<int>(value.size());
    file.write(reinterpret_cast<const char *>(&valueSize), sizeof(int));
    file.write(value.c_str(), valueSize);

    // file.close(); // Closing here is not necessary since ofstream's destructor will close the file.
}

std::string FileOrchestrator::read(const std::string &_key)
{
    auto offsetIt = indexTable.find(_key);
    if (offsetIt == indexTable.end())
    {
        return ""; // Key not found
    }

    std::ifstream file(dataFilePath, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error opening " << dataFilePath << " for reading data." << std::endl;
        return "";
    }

    file.seekg(offsetIt->second);

    int keySize;
    if (!file.read(reinterpret_cast<char *>(&keySize), sizeof(int)))
    {
        std::cerr << "Error reading key size." << std::endl;
        return "";
    }

    std::string key(keySize, '\0');
    if (!file.read(&key[0], keySize))
    {
        std::cerr << "Error reading key." << std::endl;
        return "";
    }

    int valueSize;
    if (!file.read(reinterpret_cast<char *>(&valueSize), sizeof(int)))
    {
        std::cerr << "Error reading value size." << std::endl;
        return "";
    }

    std::string value(valueSize, '\0');
    if (!file.read(&value[0], valueSize))
    {
        std::cerr << "Error reading value." << std::endl;
        return "";
    }

    return value;
}
