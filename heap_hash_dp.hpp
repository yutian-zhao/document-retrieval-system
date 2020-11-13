#ifndef HEAP_HASH_DP
#define HEAP_HASH_DP

#include <vector>

/**
 * An implementation of key-value pairs with potiential pointer to construct singly linked list.
 * @param <K> the key type
 * @param <V> the value type
 */
template <class K, class V>
class KVPair{   

public:
    K key;
    V value;
    KVPair<K, V> *next;

    KVPair(K key, V value);
};

template <class K, class V>
class Heap{
public:
    int heapSize;
    KVPair<K, V> **heap;  // double link, points to an array of pointers to pairs

    
    /**
     * Heapify (correct downwards) the ith element.
     *
     * @param int i  the index of the element to heapify
     */
    void heapify(int i);

    // constructor
    Heap(KVPair<K, V> **p, int hs);

    
    /**
     * Extract the max(root) from the heap and heapify.
     *
     * @return value
     */
    V extractMax();

    //destructor
    ~Heap();
};

template <class K, class V>
class HashTable{
    
public:
    int numBuckets, numElements;
    KVPair<K, V> **table; // double link, *table[] won't work

    // constructor
    HashTable();
    HashTable(int nb);

    /**
     * Compute the hash on the key.
     *
     * @param key   the key against which to compute the hash, must not be null
     * @return int hash value
     */
    int hashFunction(K key);

    /**
     * Insert the key and value into the hash table based on the key.
     *
     * @param key   the key against which to put into the table
     * @param value
     */
    void insert(K key, V value);

    /**
     * get the value from the hash table based on the key.
     *
     * @param key   the key of the value to searched for.
     * @return *value the pointer to the value
     */
    V* get(K key);

    // destructor
    ~HashTable();
};

/**
 * schedule the reading list based on the documents returned in a given time limit to give the max value
 *
 * @param int w time_limit
 * @param int n number of returned results
 * @param kc 2d-vector for dp
 * @param t time request of each returned doc, based on the length, in seconds
 * @param s value (relevance score) of each returned doc
 */
// return the max value (relevance score)
int bottom_up(int w, int n, std::vector<std::vector<float>> &kc, std::vector<int> &t, std::vector<float> &s);
// return a vector of doc ids, which is the scheduled reading list
std::vector<int> extract(int w, int n, std::vector<std::vector<float>> &kc, std::vector<int> &t, std::vector<float> &s);


#endif