#include "KeyValueStore.h"
#include <fstream>
#include <iostream>

KeyValueStore::KeyValueStore()
    : fileOrchestrator()
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
