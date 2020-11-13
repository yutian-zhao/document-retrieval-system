// #include <vector>
#include "hash_table.hpp"
#include <cmath>
#include <iostream>
#include <functional> 

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
int HashTable<K, V>::hashFunction(K &key){
    float a = 0.937;
    // unsigned long k = reinterpret_cast<unsigned long>(key);
    std::size_t k = std::hash<K>{}(key);
    float ka = k*a;
    return std::floor((ka - std::floor(ka))*numBuckets);
    // return reinterpret_cast<unsigned long>(key) % numBuckets;
}

template<class K, class V>
void HashTable<K, V>::insert(K &key, V &value){
    int k = hashFunction(key);
    std::cout << key << " in " << k << std::endl;
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
            std::cout << key << " in else" << std::endl;
            KVPair<K, V> *kv = new KVPair<K, V>(key, value);
            p->next = kv;
            numElements++;
        }
    }

}
template<class K, class V>
V* HashTable<K, V>::get(K &key){
    int k = hashFunction(key);
    std::cout << key << " should be in " << k << std::endl;
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

int main(int argc, char *argv[]) {
    // HashTable<int, int> *ht = new HashTable<int, int>();

    // for (int i = 1; i<6; i++){
    //     ht->insert(i,i);
    // }
    // std::cout << "num: " << ht->numElements <<std::endl;
    // for (int i = 1; i<6; i++){
    //     std::cout << *(ht->get(i)) << std::endl;
    // }
    // delete ht;

    KVPair<int, int> **h = new KVPair<int, int> *[5]();
    for (int i = 1; i<6; i++){
        h[i-1] = new KVPair<int, int>(i, i);
    }
    Heap<int, int> *hp =  new Heap<int, int>(h, 5);
    for (int i = 1; i<4; i++){
        std::cout << hp->extractMax() << std::endl;
    }
    delete hp;
}