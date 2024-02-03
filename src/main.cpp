#include <iostream>
#include "KeyValueStore.h"

using namespace std;

int main() {
    string dataFilePath = "bin/kvDB.bin";
    string indexFilePath = "bin/kvIndex.bin";

    KeyValueStore kvStore(dataFilePath, indexFilePath);

    kvStore.write("k1", "ashwani");
    kvStore.write("k2", "yadav");
    kvStore.write("k3", "iit-patna");
    kvStore.write("k4", "{ \"name\": \"Ashwani Yadav\", \"gender\": \"Male\", \"age\": 23, \"address\": { city\": \"Kanpur\", \"state\": \"UP\" }, \"favNumber\": [4, 9, 13] }");

    cout << "k1 = " << kvStore.read("k1") << endl;
    cout << "k4 = " << kvStore.read("k4") << endl;
    cout << "k5 = " << kvStore.read("k5") << endl;

    return 0;
}