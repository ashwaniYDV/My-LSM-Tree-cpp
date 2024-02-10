#include "KeyValueStore.h"
#include <fstream>
#include <iostream>

KeyValueStore::KeyValueStore()
    : engine()
{
    
}

void KeyValueStore::put(const std::string &key, const std::string &value)
{
    engine.write(key, value);
}

DataPacket KeyValueStore::get(const std::string &key)
{
    DataPacket dp;
    if (!engine.read(key, dp)) {
        std::cerr << "Key: {" << key << "} not found." << std::endl;
    }
    return dp;
}

void KeyValueStore::remove(const std::string &key) {
    engine.remove(key);
}
