#ifndef FILEORCHESTRATOR_H
#define FILEORCHESTRATOR_H

#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <filesystem>
#include <algorithm>

struct FileOrchestrator
{
    std::string rootFolder = "bin";
    std::string dataFolder = "bin/data/";
    std::string indexFolder = "bin/index/";

    std::ofstream activeStream;

    // vector to store all filenames in sorted order (latest file in index 0)
    std::vector<std::string> dataFiles;

    std::string getTimestamp() {
        // Get the current time point in milliseconds
        auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();

        std::string timestampString = std::to_string(currentTime);
        return timestampString;
    }

    std::string getCurrentDataFilePath() {
        return dataFolder + dataFiles[0];
    }

    int createFile(const std::string &filePath) {
        // Open the file for writing
        std::ofstream file(filePath);

        if (!file.is_open()) {
            std::cerr << "Error creating " << filePath << std::endl;
            return 1; // Return an error code
        }

        // Close the file
        file.close();

        return 0;
    }

    int createFolder(const std::string &folderPath) {
        if (!std::filesystem::exists(folderPath) || !std::filesystem::is_directory(folderPath)) {
            // Create the folder if it does not exist
            if (std::filesystem::create_directory(folderPath)) {
                std::cout << folderPath << " created successfully" << std::endl;
            } else {
                std::cerr << "Failed to create " << folderPath << std::endl;
                return 1; // Return an error code
            }
        }
        return 0;
    }

    int initializeFilesAndFolders() {
        createFolder(rootFolder);
        createFolder(dataFolder);
        createFolder(indexFolder);

        if (std::filesystem::is_empty(dataFolder)) {
            std::string timestamp = getTimestamp();
            createFile(dataFolder + timestamp);
        }
        if (std::filesystem::is_empty(indexFolder)) {
            createFile(indexFolder + "index");
        }

        return 0;
    }

    void createNewChunk() {
        std::string timestamp = getTimestamp();
        createFile(dataFolder + timestamp);

        // add new file name to the front of globalChunks
        dataFiles.insert(dataFiles.begin(), timestamp);

        activeStream.close(); // later it will be opened by write() function with this new file
    }

    void createNewActiveStream() {
        std::string dataFilePath = getCurrentDataFilePath();
        activeStream = std::ofstream(dataFilePath, std::ios::ate | std::ios::app | std::ios::binary);
    }

    void loadAllFiles() {
        // TODO: loop through all files in directory and add it to globalChunks
        for (const auto & entry : std::filesystem::directory_iterator(dataFolder)) {
            dataFiles.push_back(entry.path().filename().string());
        }

        std::sort(dataFiles.begin(), dataFiles.end(), std::greater<std::string>());
    }

    FileOrchestrator()
    {
        initializeFilesAndFolders();
        loadAllFiles();
        // loadIndex();
    }

    ~FileOrchestrator()
    {
        // saveIndex();
    }

    auto getActiveStream() -> std::ofstream& {
        if (!activeStream.is_open())
        {
            createNewActiveStream();
            // if (!activeStream.is_open())
            // {
            //     std::cerr << "Error opening " << getCurrentDataFilePath() << " for writing data." << std::endl;
            //     return nullptr;
            // }
        }
        return activeStream;
    }

};

// void saveIndex(const std::string &indexFileName, const std::string &indexFilePath)
// {
//     std::ofstream file(indexFilePath, std::ios::binary | std::ios::trunc);
//     if (!file.is_open())
//     {
//         std::cerr << "Error opening "<< indexFilePath << " for persisting index." << std::endl;
//         return;
//     }

//     // Write the key-value pairs to the index file in binary format
//     for (const auto &pair : globalIndexTableMapper[indexFileName])
//     {
//         int keySize = static_cast<int>(pair.first.size());
//         file.write(reinterpret_cast<const char *>(&keySize), sizeof(int));
//         file.write(pair.first.c_str(), keySize);

//         file.write(reinterpret_cast<const char *>(&pair.second), sizeof(long long));
//     }

//     file.close();

//     std::cout << "Index saved to " << indexFilePath << std::endl;
// }

// void loadIndex(const std::string &indexFileName, const std::string &indexFilePath)
// {
//     std::ifstream file(indexFilePath, std::ios::binary);
//     if (!file.is_open())
//     {
//         std::cerr << "Error opening " << indexFilePath << " for loading index." << std::endl;
//         return;
//     }

//     while (true)
//     {
//         int keySize;
//         if (!file.read(reinterpret_cast<char *>(&keySize), sizeof(int)))
//             break;

//         std::string key(keySize, '\0');
//         if (!file.read(&key[0], keySize))
//             break;

//         long long value;
//         if (!file.read(reinterpret_cast<char *>(&value), sizeof(long long)))
//             break;

//         globalIndexTableMapper[indexFileName][key] = value;
//     }

//     file.close();

//     std::cout << "Index loaded from " << indexFilePath << std::endl;
// }


#endif // FILEORCHESTRATOR_H
