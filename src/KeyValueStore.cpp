#include "KeyValueStore.h"
#include <fstream>
#include <iostream>

KeyValueStore::KeyValueStore()
    : engine()
{
}

void KeyValueStore::set(const std::string &key, const std::string &value)
{
    engine.write(key, value);
}

Record KeyValueStore::get(const std::string &key)
{
    Record record;
    if (!engine.read(key, record))
    {
        std::cerr << "Key: {" << key << "} not found." << std::endl;
    }
    return record;
}

void KeyValueStore::del(const std::string &key)
{
    engine.remove(key);
}