#include "heap_hash_dp.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
// #include <stdlib.h>
// #include <stdio.h>

using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[]) {
    if (argc < 2){
        std::cout << "Please supply the name of the text file\n";
        return 0;
    }
    int in;
    ifstream inFile(argv[1]);
    inFile >> in;
    HashTable<int, int> *ht = new HashTable<int, int>(50001);

    auto start = high_resolution_clock::now(); // Start time
    while(inFile >> in){
        ht->insert(in,in);
    }
    auto end = high_resolution_clock::now(); // End time
    auto duration = duration_cast<nanoseconds>(end - start);
    cout << "insert time: " << duration.count() << endl; // Prints program duration
    cout << "insert time per key: " << duration.count()/in << endl;
    inFile.close();

    ifstream inFile_2(argv[1]);
    inFile_2 >> in;
    start = high_resolution_clock::now();
    while(inFile_2 >> in){
        if (ht->get(in) == NULL){
            std::cout << ("cannot find ") << in << endl;
            return -1;
        }
    }
    end = high_resolution_clock::now(); // End time
    duration = duration_cast<nanoseconds>(end - start);
    cout << "search time: " << duration.count() << endl; // Prints program duration
    cout << "search time per key: " << duration.count()/in << endl;
    inFile_2.close();
    delete ht;

    cout << "Every key is inserted and found successfully " << endl;
    return 0;
}