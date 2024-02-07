#ifndef ENGINE_H
#define ENGINE_H

#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include "FileOrchestrator.h"
#include "DataPacket.h"

struct Engine
{
    FileOrchestrator orchestrator;

    std::unordered_map<std::string, std::pair<std::string, size_t>> offsetMap; 

    Engine() {
        orchestrator.loadIndex(offsetMap);
    }

    ~Engine() {
        orchestrator.saveIndex(offsetMap);
    }

    void write(const std::string &key, const std::string &value)
    {
        DataPacket dp(key, value, PacketType::UPDATE);
        std::cout << dp;

        // TODO: write a better logic for creating a new chunk
        orchestrator.checkIfNewChunkNeeded(dp.getSize());

        auto& activeStream = orchestrator.getActiveStream();

        size_t byteOffset = activeStream.tellp();

        offsetMap[dp.key] = {orchestrator.getCurrentDataFilePath(), byteOffset };
        
        activeStream << dp;
        activeStream.flush();
    }

    std::string read(const std::string &key)
    {
        if (!offsetMap.count(key))
        {
            return "";
        }

        auto dataFilePath = offsetMap[key].first;
        std::ifstream file(dataFilePath, std::ios::binary);
        if (!file.is_open())
        {
            std::cerr << "Error opening " << dataFilePath << " for reading data." << std::endl;
            return "";
        }

        auto byteOffset = offsetMap[key].second;

        file.seekg(byteOffset);

        DataPacket dp;
        file >> dp;

        std::cout << dp;

        return dp.value;
        
    }
};

#endif // ENGINE_H
