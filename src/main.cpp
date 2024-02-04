#include <iostream>
#include "KeyValueStore.h"

using namespace std;

int main() {
    string dataFilePath = "bin/kvDB.bin";
    string indexFilePath = "bin/kvIndex.bin";

    KeyValueStore kvStore(dataFilePath, indexFilePath);

    kvStore.put("k1", "ashwani");
    kvStore.put("k2", "yadav");
    kvStore.put("k3", "iit-patna");
    kvStore.put("k4", "{ \"name\": \"Ashwani Yadav\", \"gender\": \"Male\", \"age\": 23, \"address\": { city\": \"Kanpur\", \"state\": \"UP\" }, \"favNumber\": [4, 9, 13] }");

    cout << "k1 = " << kvStore.get("k1") << endl;
    cout << "k4 = " << kvStore.get("k4") << endl;
    cout << "k5 = " << kvStore.get("k5") << endl;

    return 0;
}