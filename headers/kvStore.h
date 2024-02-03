#include <fstream>
#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

// (TODO: include a serialization library like boost::serialization)

class KeyValueStore {
private: 
    unordered_map < string, long long > indexTable;
    string dataFilePath;
    string indexFilePath;

    void loadIndex() {
        ifstream file(indexFilePath, ios::binary);
        if (!file.is_open()) {
            cerr << "Error opening "<< indexFilePath << " for loading index." << endl;
            return;
        }

        // This loop reads data from binary file. 
        // It first reads key size, then allocates a string of that size and reads the key itself. 
        // Next, it reads the long long value. 
        // If any read operation fails, it breaks out of the loop.
        while (true) {
            int keySize;
            if (!file.read(reinterpret_cast<char*>(&keySize), sizeof(int)))
                break;  // Break if unable to read key size

            string key(keySize, '\0');
            if (!file.read(&key[0], keySize))
                break;  // Break if unable to read key

            long long value;
            if (!file.read(reinterpret_cast<char*>(&value), sizeof(long long)))
                break;  // Break if unable to read value

            indexTable[key] = value;
        }

        // Close the file
        file.close();

        cout << "Index loaded from " << indexFilePath << endl;
    }

    void saveIndex() {
        ofstream file(indexFilePath, ios::binary | ios::trunc);
        if (!file.is_open()) {
            cerr << "Error opening Index file for writing index." << endl;
            return;
        }

        // For each k-v pair, it writes the key size (as an integer) and the key itself in binary format. 
        // Then, it writes the corresponding long long value in binary format.
        for (const auto& pair : indexTable) {
            int keySize = static_cast<int>(pair.first.size());
            file.write(reinterpret_cast<const char*>(&keySize), sizeof(int)); // write key-size
            file.write(pair.first.c_str(), keySize); // write key value

            file.write(reinterpret_cast<const char*>(&pair.second), sizeof(long long)); // write value
        }

        // Close the file
        file.close();

        cout << "Index saved to " << indexFilePath << endl;
    }

public: 
    KeyValueStore(const string& dataFilePath, const string& indexFilePath)
        : dataFilePath(dataFilePath), indexFilePath(indexFilePath) {
        loadIndex();
    }

    ~KeyValueStore() {
        saveIndex();
    }
    
    void write(const string & key, const string & value) {
        // open file in append mode (ios::ate = point to end of file)
        ofstream file(dataFilePath, ios::ate | ios::app | ios::binary);

        if (!file.is_open()) {
            cerr << "Error opening "<< dataFilePath << " for writing data." << endl;
            return;
        }

        long long byteOffset = file.tellp();
        // Check if getting the byte byteOffset was successful
        if (byteOffset == -1) {
            cerr << "Failed to get byte offset." << endl;
            return;
        }

        // store pointer in hash table
        indexTable[key] = byteOffset;

        // write key
        int keySize = static_cast<int>(key.size());
        file.write(reinterpret_cast<const char*>(&keySize), sizeof(int)); // write key-size
        file.write(key.c_str(), keySize); // write key value

        // write value
        int valueSize = static_cast<int>(value.size());
        file.write(reinterpret_cast<const char*>(&valueSize), sizeof(int)); // write value-size
        file.write(value.c_str(), valueSize); // write value value

        
        // file.close();
    }


    string read(const string & key) {
        auto offsetIt = indexTable.find(key);
        if (offsetIt == indexTable.end()) {
            return ""; // Key not found
        }

        ifstream file(dataFilePath, ios::binary);
        if (!file.is_open()) {
            cerr << "Error opening "<< dataFilePath << " for reading data." << endl;
            return "";
        }

        file.seekg(offsetIt -> second);
        
        int keySize;
        if (!file.read(reinterpret_cast<char*>(&keySize), sizeof(int))) {
            cerr << "Error reading key size." << endl;
            return "";
        }

        string key2(keySize, '\0');
        if (!file.read(&key2[0], keySize)) {
            cerr << "Error reading key." << endl;
            return "";
        }

        int valueSize;
        if (!file.read(reinterpret_cast<char*>(&valueSize), sizeof(int))) {
            cerr << "Error reading value size." << endl;
            return "";
        }

        string value(valueSize, '\0');
        if (!file.read(&value[0], valueSize)) {
            cerr << "Error reading value." << endl;
            return "";
        }

        return value;
    }
};