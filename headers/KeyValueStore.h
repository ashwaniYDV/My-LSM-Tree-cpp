#pragma once

#include <unordered_map>
#include <string>
#include "Engine.h"

class KeyValueStore
{
private:
    Engine engine;

public:
    KeyValueStore();

    void set(const std::string &key, const std::string &value);
    Record get(const std::string &key);
    void del(const std::string &key);
};
