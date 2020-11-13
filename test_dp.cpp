#include "heap_hash_dp.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>

using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[]) {
    if (argc < 2){
        std::cout << "Please supply the name of the text file\n";
        return 0;
    }
    ifstream inFile(argv[1]);
    int w;
    int n;
    int temp;
    inFile >> w;
    inFile >> n;
    vector<int> t;
    vector<float> s;
    vector<int> answer;
    for (int i=0; i<n; i++){
        inFile >> temp;
        t.push_back(temp);
    }
    for (int i=0; i<n; i++){
        inFile >> temp;
        s.push_back(temp);
    }
    for (int i=0; i<n; i++){
        inFile >> temp;
        answer.push_back(temp);
    }

    vector<vector<float>> kc(n+1, vector<float>(w+1));
    for (int j=0; j<= w; j++){
        kc[0][j] = 0; 
    }

    auto start = high_resolution_clock::now(); // Start time
    int sum = bottom_up(w, n, kc, t, s);
    vector<int> results = extract(w, n, kc, t, s);
    auto end = high_resolution_clock::now(); // End time
    auto duration = duration_cast<nanoseconds>(end - start);
    cout << "dp used time: " << duration.count() << endl;

    for (int i=0; i<results.size(); i++){
        if (answer[results[i]] != 1){
            cout << "Wrong anwer." << endl;
            return -1;
        }
    }

    cout << "dp solved tests correctly." << endl;
    return 0;

}