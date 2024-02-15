#pragma once

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

enum RecordType
{
    UPDATE,
    DELETE
};

struct Record
{
    time_t timestamp;
    size_t keySize;
    size_t valueSize;
    RecordType recordType;
    std::string key;
    std::string value;
    unsigned char checksum;

    Record(
        const time_t timestamp,
        const std::string &key,
        const std::string &value,
        const RecordType &rType)
        : timestamp(timestamp), keySize(key.size()), valueSize(value.size()), recordType(rType), key(key), value(value) {}

    Record(
        const std::string &key,
        const std::string &value,
        const RecordType &rType) : keySize(key.size()), valueSize(value.size()), recordType(rType), key(key), value(value)
    {
        timestamp = system_clock::to_time_t(system_clock::now());
    }

    Record()
    {
    }

    Record(const Record &other)
    {
        *this = std::move(Record(other));
    }

    friend auto operator<<(ofstream &fs, const Record &record) -> ofstream &
    {
        // Calculate and set the checksum on the original record
        // const_cast<Record &>(record).calculateChecksum(); is used in C++ to call a non-const member function (calculateChecksum()) on a const object (record). 
        // directly calling calculateChecksum() on a const object is not allowed because it would modify the object, violating its const-ness.
        const_cast<Record &>(record).calculateChecksum();

        fs.write(reinterpret_cast<const char *>(&record.timestamp), sizeof(record.timestamp));
        fs.write(reinterpret_cast<const char *>(&record.keySize), sizeof(record.keySize));
        fs.write(reinterpret_cast<const char *>(&record.valueSize), sizeof(record.valueSize));
        fs.write(reinterpret_cast<const char *>(&record.recordType), sizeof(record.recordType));
        fs.write(record.key.c_str(), record.keySize);
        fs.write(record.value.c_str(), record.valueSize);
        fs.write(reinterpret_cast<const char *>(&record.checksum), sizeof(record.checksum));

        return fs;
    }

    friend auto operator<<(ostream &os, const Record &record) -> ostream &
    {
        os << "{timestamp: " << record.timestamp << ", recordType: " << record.recordType << ", keySize: " << record.keySize << ", valueSize: " << record.valueSize << ", key: " << record.key << ", value: " << record.value << "}" << std::endl;
        return os;
    }

    friend auto operator>>(istream &is, Record &record) -> istream &
    {
        is.read(reinterpret_cast<char *>(&record.timestamp), sizeof(record.timestamp));
        is.read(reinterpret_cast<char *>(&record.keySize), sizeof(record.keySize));
        is.read(reinterpret_cast<char *>(&record.valueSize), sizeof(record.valueSize));
        is.read(reinterpret_cast<char *>(&record.recordType), sizeof(record.recordType));

        std::string key(record.keySize, '\0');
        std::string value(record.valueSize, '\0');
        is.read(&key[0], record.keySize);
        is.read(&value[0], record.valueSize);
        record.key = key;
        record.value = value;

        // Read checksum from the file
        is.read(reinterpret_cast<char *>(&record.checksum), sizeof(record.checksum));

        unsigned char storedChecksum = record.checksum;
        record.calculateChecksum();

        if (record.checksum != storedChecksum)
        {
            std::cerr << "Checksum mismatch. Record is corrupted." << std::endl;
        }

        return is;
    }

    auto operator==(const Record &record) const -> bool
    {
        return (
            record.recordType == this->recordType and
            record.timestamp == this->timestamp and
            record.keySize == this->keySize and
            record.valueSize == this->valueSize and
            record.key == this->key and
            record.value == this->value);
    }

    auto getSize() const -> size_t
    {
        return sizeof(time_t) + sizeof(keySize) + sizeof(valueSize) + sizeof(RecordType) + keySize + valueSize;
    }

    // simple XOR checksum
    // TODO: use better cryptographic hash functions
    auto calculateChecksum() -> void
    {
        checksum = 0;

        // XOR the timestamp bytes
        for (size_t i = 0; i < sizeof(timestamp); ++i)
        {
            checksum ^= static_cast<unsigned char>((timestamp >> (i * 8)) & 0xFF);
        }

        // XOR the keySize bytes
        for (size_t i = 0; i < sizeof(keySize); ++i)
        {
            checksum ^= static_cast<unsigned char>((keySize >> (i * 8)) & 0xFF);
        }

        // XOR the valueSize bytes
        for (size_t i = 0; i < sizeof(valueSize); ++i)
        {
            checksum ^= static_cast<unsigned char>((valueSize >> (i * 8)) & 0xFF);
        }

        // XOR the recordType
        checksum ^= static_cast<unsigned char>(recordType);

        // XOR each byte of the key
        for (const auto &c : key)
        {
            checksum ^= static_cast<unsigned char>(c);
        }

        // XOR each byte of the value
        for (const auto &c : value)
        {
            checksum ^= static_cast<unsigned char>(c);
        }
    }
};
