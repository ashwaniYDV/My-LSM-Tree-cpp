#ifndef FILEORCHESTRATOR_H
#define FILEORCHESTRATOR_H

#include <unordered_map>
#include <string>

class FileOrchestrator
{
private:
    std::unordered_map<std::string, long long> indexTable;
    std::string dataFilePath;
    std::string indexFilePath;

    void loadIndex();
    void saveIndex();

public:
    FileOrchestrator(const std::string &dataFilePath, const std::string &indexFilePath);
    ~FileOrchestrator();

    void write(const std::string &key, const std::string &value);
    std::string read(const std::string &key);
};

#endif // FILEORCHESTRATOR_H
