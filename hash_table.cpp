#include <vector>
#include "hash_table.hpp"
#include <cmath>
#include <iostream>
#include <functional> 

#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

using namespace std;

template <class K, class V>
KVPair<K, V>::KVPair(K k, V v){
    key = k;  // name should be different
    value = v;
    next = NULL;
}

template <class K, class V>
void Heap<K, V>::heapify(int i){
    int l = i*2+1;
    int r = i*2+2;
    int largest = i;
    if (l < heapSize && heap[l]->key > heap[i]->key){
        largest = l;
    }
    if (r < heapSize && heap[r]->key > heap[largest]->key){
        largest = r;
    }
    if (largest != i){
         KVPair<K, V> *p = heap[largest];
         heap[largest] = heap[i];
         heap[i] = p;
         heapify(largest);
    }
}

template <class K, class V>
Heap<K, V>::Heap(KVPair<K, V> **p, int hs){
    heap = p;
    heapSize = hs;
    for (int i = std::floor(heapSize/2); i>=0; i--){
        heapify(i);
    }
}

template <class K, class V>
V Heap<K, V>::extractMax(){
    if (heapSize <= 0){
        throw "Empty Heap!";
    }
    V max = heap[0]->value;
    // std::cout << "the end: " << heap[heapSize-1]->value << std::endl;
    delete heap[0];  // 
    heap[0] = heap[heapSize-1];
    heap[heapSize-1] = NULL;
    heapSize = heapSize-1;
    heapify(0);
    // std::cout << "new begin: " << heap[0]->value << std::endl;
    return max;
}

template <class K, class V>
Heap<K, V>::~Heap(){
    for (int i = 0; i < heapSize; i++){
        delete heap[i];
        heap[i] = NULL;
    }
    delete [] heap; // 
}

template <class K, class V>
HashTable<K, V>::HashTable(){
    numBuckets = 2083;
    numElements = 0;
    table = new KVPair<K, V> *[numBuckets]();
}

template <class K, class V>
HashTable<K, V>::HashTable(int nb){
    numBuckets = nb;
    numElements = 0;
    table = new KVPair<K, V> *[numBuckets]();  // () is a must to initialise to null
}

template<class K, class V>
int HashTable<K, V>::hashFunction(K key){
    // float a = 0.937;
    // std::size_t k = std::hash<K>{}(key);
    // std::cout << key << " has hash: " << k << std::endl;
    // float ka = k*a;
    // return std::floor(((float)(ka - std::floor(ka)))*numBuckets);
    std::size_t k = std::hash<K>{}(key);
    return k%numBuckets;
    // return reinterpret_cast<unsigned long>(key) % numBuckets;
}

template<class K, class V>
void HashTable<K, V>::insert(K key, V value){
    int k = hashFunction(key);
    // std::cout << key << " in " << k << std::endl;
    KVPair<K, V> *p = table[k];
    if (p == NULL){
        KVPair<K, V> *kv = new KVPair<K, V>(key, value);
        table[k] = kv;
        numElements++;
    } else {
        while (p->next != NULL){
            if (p->key == key){
                break;
            }
            p = p->next;
        }
        if (p->key == key){
            p->value = value;
        } else {
            // std::cout << key << " in else" << std::endl;
            KVPair<K, V> *kv = new KVPair<K, V>(key, value);
            p->next = kv;
            numElements++;
        }
    }

}
template<class K, class V>
V* HashTable<K, V>::get(K key){
    int k = hashFunction(key);
    // std::cout << key << " should be in " << k << std::endl;
    KVPair<K, V> *p = table[k];
    while (p != NULL){
        if (p->key == key){
            return &(p->value);
        }
        p = p->next;
    }
    return NULL;
}

template<class K, class V>
HashTable<K, V>::~HashTable() {
    for (int i = 0; i < numBuckets; i++){
        KVPair<K, V> *p = table[i];
        while (p != NULL){
            KVPair<K, V> *prev = p;
            p = p->next;
            delete prev;
        }
        table[i] = NULL;
    }
    delete [] table;
}

// int bottom_up(int w, int n, vector<vector<float>> &kc, vector<int> &t, vector<float> &s){
//     for (int i = 1; i<= n; i++){
//         for (int j = 0; j<=w; j++){
//             if (j-t[i-1] >= 0 && (s[i-1]+kc[i-1][j-t[i-1]]>kc[i-1][j])){
//                 kc[i][j] = s[i-1]+kc[i-1][j-t[i-1]];
//             } else{
//                 kc[i][j] = kc[i-1][j];
//             }
//         }
//     }
//     return kc[n][w];
// }

// vector<int> extract(int w, int n, vector<vector<float>> &kc, vector<int> &t, vector<float> &s){
//     vector<int> docs;
//     while(w>=0 && n>=0 && kc[n][w] > 0){
//         if (kc[n][w] > kc[n-1][w]){
//             docs.push_back(n-1);
//             w = w-t[n-1];
//             n = n-1; 
//         } else {
//             n = n-1;
//         }
//     }
//     return docs;
// }

template class KVPair<int, int>;
template class KVPair<float, int>;
template class Heap<int, int>;
template class Heap<float, int>;
template class HashTable<string, int>;
template class HashTable<int, int>;
// template class HashTable<int, float>;
template class HashTable<string, HashTable<int, int>*>;
template class HashTable<int, HashTable<string, int>*>;

// int main(int argc, char *argv[]) {
//     HashTable<int, int> *ht = new HashTable<int, int>();

//     for (int i = 1; i<2000; i++){
//         ht->insert(i,i);
//     }
//     std::cout << "num: " << ht->numElements <<std::endl;
//     // for (int i = 1; i<6; i++){
//     //     std::cout << *(ht->get(i)) << std::endl;
//     // }
//     delete ht;

//     // KVPair<int, int> **h = new KVPair<int, int> *[5]();
//     // for (int i = 1; i<6; i++){
//     //     h[i-1] = new KVPair<int, int>(i, i);
//     // }
//     // Heap<int, int> *hp =  new Heap<int, int>(h, 5);
//     // for (int i = 1; i<4; i++){
//     //     std::cout << hp->extractMax() << std::endl;
//     // }
//     // delete hp;

//     // vector<int> t = {77, 22, 29, 50, 99};
//     // vector<float> s = {92, 22, 87, 46, 90};
//     // int w = 100; 
//     // int n = 5;
//     // vector<vector<float>> kc(n+1, vector<float>(w+1));
//     // for (int j=0; j<= w; j++){
//     //     kc[0][j] = 0; 
//     // }

//     // int sum = bottom_up(w, n, kc, t, s);
//     // cout << "sum: " << sum << endl;
//     // vector<int> results = extract(w, n, kc, t, s);
//     // for (int i=0; i<results.size(); i++){
//     //     cout << results[i] << endl;
//     // }
// }
