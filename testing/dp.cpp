#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

struct DataPacket {
    time_t timestamp;
    size_t keySize;
    size_t valueSize;
    std::string key;
    std::string value;
};

void writeDataPacket(const DataPacket& packet, std::ofstream& file) {
    file.write(reinterpret_cast<const char*>(&packet.timestamp), sizeof(time_t));
    file.write(reinterpret_cast<const char*>(&packet.keySize), sizeof(size_t));
    file.write(reinterpret_cast<const char*>(&packet.valueSize), sizeof(size_t));
    file.write(packet.key.c_str(), packet.keySize);
    file.write(packet.value.c_str(), packet.valueSize);
}

void readDataPacket(DataPacket& packet, std::ifstream& file) {
    file.read(reinterpret_cast<char*>(&packet.timestamp), sizeof(time_t));
    file.read(reinterpret_cast<char*>(&packet.keySize), sizeof(size_t));
    file.read(reinterpret_cast<char*>(&packet.valueSize), sizeof(size_t));

    // Allocate memory for key and value
    char keyBuffer[packet.keySize];
    char valueBuffer[packet.valueSize];

    // Read key and value
    file.read(keyBuffer, packet.keySize);
    file.read(valueBuffer, packet.valueSize);

    // Assign to packet
    packet.key.assign(keyBuffer, packet.keySize);
    packet.value.assign(valueBuffer, packet.valueSize);
}

int main() {
    // Example usage
    std::ofstream outputFile("data.bin", std::ios::binary | std::ios::out);
    DataPacket packet1 = {std::time(0), 6, 8, "Key123", "Value456"};
    writeDataPacket(packet1, outputFile);
    outputFile.close();

    std::ifstream inputFile("data.bin", std::ios::binary | std::ios::in);
    DataPacket packet2;
    readDataPacket(packet2, inputFile);
    inputFile.close();

    // Access the read data
    std::cout << "Timestamp: " << packet2.timestamp << std::endl;
    std::cout << "Key: " << packet2.key << std::endl;
    std::cout << "Value: " << packet2.value << std::endl;

    return 0;
}
