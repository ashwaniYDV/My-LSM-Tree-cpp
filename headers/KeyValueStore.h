#ifndef KEYVALUESTORE_H
#define KEYVALUESTORE_H

#include <unordered_map>
#include <string>
#include "Engine.h"

class KeyValueStore
{
private:
    Engine engine;
public:
    KeyValueStore();

    void put(const std::string &key, const std::string &value);
    DataPacket get(const std::string &key);
};

#endif // KEYVALUESTORE_H
