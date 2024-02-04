#include "FileOrchestrator.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>

std::string FileOrchestrator::getTimestamp() {
    // Get the current time point in milliseconds
    auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    // Print the current 13-digit UTC timestamp
    std::cout << "Current 13-digit UTC timestamp: " << currentTime << std::endl;
    std::string timestampString = std::to_string(currentTime);
    return timestampString;
}

int FileOrchestrator::createFile(const std::string &filePath) {
    // Open the file for writing
    std::ofstream batFile(filePath);

    if (!batFile.is_open()) {
        std::cerr << "Error creating " << filePath << std::endl;
        return 1; // Return an error code
    }

    // Close the file
    batFile.close();

    return 0;
}

void FileOrchestrator::loadAllIndex() {
    // for (const auto & entry : std::filesystem::directory_iterator("./")) {
    //     std::cout << entry.path() << std::endl;
    // }

    globalFiles.push_back("1707054019214");

    std::sort(globalFiles.begin(), globalFiles.end(), std::greater<std::string>());

    for (const auto& file : globalFiles) {
        loadIndex(file, indexFolder + file);
    }
}

void FileOrchestrator::saveAllIndex() {
    for (const auto& file : globalFiles) {
        saveIndex(file, indexFolder + file);
    }
}

int FileOrchestrator::initializeFilesAndFolders() {

    if (!std::filesystem::exists(dataFolder) || !std::filesystem::is_directory(dataFolder)) {
        // Create the folder if it does not exist
        if (std::filesystem::create_directory(dataFolder)) {
            std::cout << dataFolder << " created successfully" << std::endl;
        } else {
            std::cerr << "Failed to create " << dataFolder << std::endl;
            return 1; // Return an error code
        }
    } else {
        std::cout << dataFolder << " already exists" << std::endl;
    }

    if (!std::filesystem::exists(indexFolder) || !std::filesystem::is_directory(indexFolder)) {
        // Create the folder if it does not exist
        if (std::filesystem::create_directory(indexFolder)) {
            std::cout << indexFolder << " created successfully" << std::endl;
        } else {
            std::cerr << "Failed to create " << indexFolder << std::endl;
            return 1; // Return an error code
        }
    } else {
        std::cout << indexFolder << " already exists" << std::endl;
    }

    if (std::filesystem::is_empty(dataFolder)) {
        createFile(dataFolder + getTimestamp());
        createFile(indexFolder + getTimestamp());
    }

    return 0;
}

std::string FileOrchestrator::getCurrentDataFilePath() {
    return dataFolder + globalFiles[0];
}

std::string FileOrchestrator::getCurrentDataFileName() {
    return globalFiles[0];
}

void FileOrchestrator::createNew() {
    std::string dataFilePath = getCurrentDataFilePath();
    activeStream = std::ofstream(dataFilePath, std::ios::ate | std::ios::app | std::ios::binary);
}

FileOrchestrator::FileOrchestrator()
{
    initializeFilesAndFolders();
    loadAllIndex();
}

FileOrchestrator::~FileOrchestrator()
{
    saveAllIndex();
}

void FileOrchestrator::saveIndex(const std::string &indexFileName, const std::string &indexFilePath)
{
    std::ofstream file(indexFilePath, std::ios::binary | std::ios::trunc);
    if (!file.is_open())
    {
        std::cerr << "Error opening "<< indexFilePath << " for persisting index." << std::endl;
        return;
    }

    // Write the key-value pairs to the index file in binary format
    for (const auto &pair : globalIndexTable[indexFileName])
    {
        int keySize = static_cast<int>(pair.first.size());
        file.write(reinterpret_cast<const char *>(&keySize), sizeof(int));
        file.write(pair.first.c_str(), keySize);

        file.write(reinterpret_cast<const char *>(&pair.second), sizeof(long long));
    }

    file.close();

    std::cout << "Index saved to " << indexFilePath << std::endl;
}

void FileOrchestrator::loadIndex(const std::string &indexFileName, const std::string &indexFilePath)
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

        globalIndexTable[indexFileName][key] = value;
    }

    file.close();

    std::cout << "Index loaded from " << indexFilePath << std::endl;
}

void FileOrchestrator::write(const std::string &key, const std::string &value)
{
    if (!activeStream.is_open())
    {
        createNew();
        if (!activeStream.is_open()) {
            std::cerr << "Error opening " << getCurrentDataFilePath() << " for writing data." << std::endl;
            return;
        }
    }

    long long byteOffset = activeStream.tellp();

    globalIndexTable[getCurrentDataFileName()][key] = byteOffset;

    int keySize = static_cast<int>(key.size());
    activeStream.write(reinterpret_cast<const char *>(&keySize), sizeof(int));
    activeStream.flush();
    activeStream.write(key.c_str(), keySize);
    activeStream.flush();

    int valueSize = static_cast<int>(value.size());
    activeStream.write(reinterpret_cast<const char *>(&valueSize), sizeof(int));
    activeStream.flush();
    activeStream.write(value.c_str(), valueSize);
    activeStream.flush();
}

std::string FileOrchestrator::read(const std::string &_key)
{
    for (const auto& fileName : globalFiles) {
        auto offsetIt = globalIndexTable[fileName].find(_key);
        if (offsetIt == globalIndexTable[fileName].end())
        {
            continue; // Key not found
        }

        std::string dataFilePath = dataFolder + fileName;
        std::ifstream file(dataFilePath, std::ios::binary);
        if (!file.is_open())
        {
            std::cerr << "Error opening " << dataFilePath << " for reading data." << std::endl;
            continue;
        }

        file.seekg(offsetIt->second);

        int keySize;
        if (!file.read(reinterpret_cast<char *>(&keySize), sizeof(int)))
        {
            std::cerr << "Error reading key size." << std::endl;
            continue;
        }

        std::string key(keySize, '\0');
        if (!file.read(&key[0], keySize))
        {
            std::cerr << "Error reading key." << std::endl;
            continue;
        }

        int valueSize;
        if (!file.read(reinterpret_cast<char *>(&valueSize), sizeof(int)))
        {
            std::cerr << "Error reading value size." << std::endl;
            continue;
        }

        std::string value(valueSize, '\0');
        if (!file.read(&value[0], valueSize))
        {
            std::cerr << "Error reading value." << std::endl;
            continue;
        }

        return value;
    }
    return "";
}
