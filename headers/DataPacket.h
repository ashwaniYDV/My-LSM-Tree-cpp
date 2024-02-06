#ifndef DATAPACKET_H
#define DATAPACKET_H

#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

using std::istream;
using std::ofstream;
using std::ostream;
using std::size_t;
using std::time_t;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::system_clock;

enum PacketType
{
    UPDATE,
    DELETE
};

// Overload << operator for PacketType enum
// std::ostream& operator<<(std::ostream& os, PacketType c) {
//     switch (c) {
//         case UPDATE:
//             os << "UPDATE"; 
//             break;
//         case DELETE: 
//             os << "DELETE"; 
//             break;
//         default: 
//             os << ""; 
//             break;
//     }
//     return os;
// }

struct DataPacket
{
    time_t timestamp;
    size_t keySize;
    size_t valueSize;
    PacketType packetType;
    std::string key;
    std::string value;

    DataPacket(
        const time_t timestamp,
        const std::string &key,
        const std::string &value,
        const PacketType &pType)
        : timestamp(timestamp), keySize(key.size()), valueSize(value.size()), packetType(pType), key(key), value(value) {}

    DataPacket(
        const std::string &key,
        const std::string &value,
        const PacketType &pType) : keySize(key.size()), valueSize(value.size()), packetType(pType), key(key), value(value)
    {
        timestamp = system_clock::to_time_t(system_clock::now());
    }

    DataPacket() {
        
    }
    
    DataPacket(const DataPacket &other)
    {
        *this = std::move(DataPacket(other));
    }

    friend auto operator<<(ofstream &fs, const DataPacket &dp) -> ofstream &
    {
        fs.write(reinterpret_cast<const char *>(&dp.timestamp), sizeof(dp.timestamp));
        fs.flush();
        fs.write(reinterpret_cast<const char *>(&dp.keySize), sizeof(dp.keySize));
        fs.flush();
        fs.write(reinterpret_cast<const char *>(&dp.valueSize), sizeof(dp.valueSize));
        fs.flush();
        fs.write(reinterpret_cast<const char *>(&dp.packetType), sizeof(dp.packetType));
        fs.flush();
        fs.write(dp.key.c_str(), dp.keySize);
        fs.flush();
        fs.write(dp.value.c_str(), dp.valueSize);
        fs.flush();
        
        return fs;
    }

    friend auto operator<<(ostream &os, const DataPacket &dp) -> ostream &
    {
        os << "{timestamp: " << dp.timestamp << ", packetType: " << dp.packetType << ", keySize: " << dp.keySize << ", valueSize: " << dp.valueSize << ", key: " << dp.key << ", value: " << dp.value << "}" << std::endl;
        return os;
    }

    friend auto operator>>(istream &is, DataPacket &dp) -> istream &
    {
        is.read(reinterpret_cast<char *>(&dp.timestamp), sizeof(dp.timestamp));
        is.read(reinterpret_cast<char *>(&dp.keySize), sizeof(dp.keySize));
        is.read(reinterpret_cast<char *>(&dp.valueSize), sizeof(dp.valueSize));
        is.read(reinterpret_cast<char *>(&dp.packetType), sizeof(dp.packetType));

        std::string key(dp.keySize, '\0');
        std::string value(dp.valueSize, '\0');
        is.read(&key[0], dp.keySize);
        is.read(&value[0], dp.valueSize);
        dp.key = key;
        dp.value = value;
        
        return is;
    }

    auto operator==(const DataPacket &dp) const -> bool
    {
        return (
            dp.packetType == this->packetType and
            dp.timestamp == this->timestamp and
            dp.keySize == this->keySize and
            dp.valueSize == this->valueSize and
            dp.key == this->key and
            dp.value == this->value);
    }

    auto getSize() const -> size_t
    {
        return sizeof(time_t) + 2 * sizeof(size_t) + sizeof(PacketType) + keySize + valueSize;
    }
};

#endif // DATAPACKET_H
