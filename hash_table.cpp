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
    HashTable<int, int> *ht = new HashTable<int, int>();

    for (int i = 1; i<6; i++){
        ht->insert(i,i);
    }
    std::cout << "num: " << ht->numElements <<std::endl;
    for (int i = 1; i<6; i++){
        std::cout << *(ht->get(i)) << std::endl;
    }
    delete ht;
}