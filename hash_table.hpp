#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <vector>

/**
 * An implementation of the hash table abstract data stucture.
 * @param <K> the key type for the hash table
 * @param <V> the value type for the hash table
 */
template <class K, class V>
class HashTable{
    class KVPair{   
    private:
        K key;
        V value;
    public:
        KVPair(K key, V value){
            key = key;
            value = value;
        }
    };

private:
    int numBuckets, numElements;
    std::vector<KVPair> *table;

public:
    HashTable(int numBuckets){
        numBuckets = numBuckets;
        numElements = 0;
        table = new std::vector<KVPair>[numBuckets];
    }

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
     * Delete the key and value from the hash table based on the key.
     *
     * @param key   the key against which to delete from the table
     * @param value
     */
    void delect(K key);

    /**
     * get the value from the hash table based on the key.
     *
     * @param key   the key of the value to searched for.
     * @return value
     */
    V get(K key);

    std::vector<KVPair> getItems();

    std::vector<K> getKeys();

    std::vector<V> getValues();
};

#endif