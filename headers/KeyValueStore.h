#ifndef KEYVALUESTORE_H
#define KEYVALUESTORE_H

#include <unordered_map>
#include <string>

class KeyValueStore
{
private:
    std::unordered_map<std::string, long long> indexTable;
    std::string dataFilePath;
    std::string indexFilePath;

    void loadIndex();
    void saveIndex();

public:
    KeyValueStore(const std::string &dataFilePath, const std::string &indexFilePath);
    ~KeyValueStore();

    void write(const std::string &key, const std::string &value);
    std::string read(const std::string &key);
};

#endif // KEYVALUESTORE_H
