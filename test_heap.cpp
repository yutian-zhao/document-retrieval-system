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
    int len;
    int in;
    ifstream inFile(argv[1]);
    inFile >> len;
    KVPair<int, int> **h = new KVPair<int, int> *[len]();
    int i = 0;
    while(inFile >> in){
        h[i] = new KVPair<int, int>(in, in);
        i = i+1;
    }

    auto start = high_resolution_clock::now(); // Start time
    Heap<int, int> *hp =  new Heap<int, int>(h, len);

    int prev = -1;
    for (int i=0; i<100; i++){ //100
        int cur = hp->extractMax();
        if (cur > prev && prev != -1){
            std::cout << "Heap isn't sorted correctly.\n";
            return -1;
        }
        prev = cur;
    }
    auto end = high_resolution_clock::now(); // End time
    auto duration = duration_cast<nanoseconds>(end - start);
    cout << "build time + extract 50 max elements time: " << duration.count() << endl; // Prints program duration
    delete hp;
    inFile.close();

    cout << "Heap is built correctly and 50 max elements are extracted correctly. " << endl;
    return 0;
}