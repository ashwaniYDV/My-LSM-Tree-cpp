#include "KeyValueStore.h"
#include <fstream>
#include <iostream>

KeyValueStore::KeyValueStore(const std::string &dataFilePath, const std::string &indexFilePath)
    : fileOrchestrator(dataFilePath, indexFilePath)
{
    
}

void KeyValueStore::put(const std::string &key, const std::string &value)
{
    fileOrchestrator.write(key, value);
}

std::string KeyValueStore::get(const std::string &key)
{
    return fileOrchestrator.read(key);
}
