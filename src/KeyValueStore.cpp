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

std::string KeyValueStore::get(const std::string &key)
{
    return engine.read(key);
}
