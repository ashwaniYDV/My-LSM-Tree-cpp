#ifndef KEYVALUESTORE_H
#define KEYVALUESTORE_H

#include <unordered_map>
#include <string>
#include "FileOrchestrator.h"

class KeyValueStore
{
private:
    FileOrchestrator fileOrchestrator;
public:
    KeyValueStore(const std::string &dataFilePath, const std::string &indexFilePath);

    void put(const std::string &key, const std::string &value);
    std::string get(const std::string &key);
};

#endif // KEYVALUESTORE_H
