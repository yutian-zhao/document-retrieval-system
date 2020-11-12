#include <vector>

template <class K, class V>
class HashTable{
    class KVPair{   
    private:
        K key;
        V value;
    public:
        KVPair(K key, V=value){
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

    int hashFunction(K key);

    void insert(K key, V value);

    void delect(K key);

    V get(K key);

    std::vector<KVPair> getItems();

    std::vector<K> getKeys();

    std::vector<V> getValues();
};