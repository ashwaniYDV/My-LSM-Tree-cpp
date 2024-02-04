#ifndef FILEORCHESTRATOR_H
#define FILEORCHESTRATOR_H

#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>

class FileOrchestrator
{
private:
    std::ofstream activeStream;
    std::string dataFolder = "bin/data/";
    std::string indexFolder = "bin/index/";
    std::unordered_map<std::string, std::unordered_map<std::string, long long>> globalIndexTable;
    std::vector<std::string> globalFiles;
    

    std::string getTimestamp();
    std::string getCurrentDataFilePath();
    std::string getCurrentDataFileName();
    int initializeFilesAndFolders();
    int createFile(const std::string &filePath);
    void createNew();

    void loadAllIndex();
    void saveAllIndex();

    void loadIndex(const std::string &indexFileName, const std::string &indexFilePath);
    void saveIndex(const std::string &indexFileName, const std::string &indexFilePath);

public:
    FileOrchestrator();
    ~FileOrchestrator();

    void write(const std::string &key, const std::string &value);
    std::string read(const std::string &key);
};

#endif // FILEORCHESTRATOR_H
