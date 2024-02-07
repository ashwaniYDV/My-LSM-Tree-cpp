#include <iostream>
#include "KeyValueStore.h"

using namespace std;

int main() {
    KeyValueStore kvStore;

    int n;
    cin >> n;
    for(int i = 1; i <= n; i++) {
        std::string key = std::to_string(i);
        kvStore.put(key, key);
    }

    for(int i = 1; i <= n; i++) {
        std::string key = std::to_string(i);
        std::cout << kvStore.get(key).value << std::endl;
    }

    return 0;

    while(true) {
        cout << "Enter 0 to exit. Enter 1 to get item.  Enter 2 to put item" << std::endl;
        
        int choice;
        cin >> choice;
        if (choice == 0) {
            break;
        } else if (choice == 1) {
            cout << "Enter key: ";
            std::string key;
            cin >> key;
            std::cout << kvStore.get(key).value << std::endl;
        } else if (choice == 2) {
            cout << "Enter key: ";
            std::string key;
            cin >> key;
            cout << "Enter value: ";
            std::string value;
            cin >> value;
            kvStore.put(key, value);
        } else {
            std::cout << "Enter correct choice." << std::endl;
        }
    }

    return 0;
}