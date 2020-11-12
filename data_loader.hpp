#ifndef DATA_LOADER_HPP
#define DATA_LOADER_HPP

#include <fstream>
#include <vector>
#include <string>
#include "hash_table.hpp"

struct doc{
    int id, len;
    std::string title, content, author, tag;
};

class DataLoader {
private:
    HashTable<int, HashTable<std::string, int>> doc_to_token_counts;
    HashTable<std::string, HashTable<int, int>> token_to_doc_counts;
    int numDocs;
    HashTable<int, struct doc> docs;

public:
    DataLoader (std::string read_dir);
    /**
     * Preprocessing the input text and return a list of tokens.
     *
     * @param string text   the text to be processed
     * @param a list of tokens
     */
    std::vector<std::string> preprocess (std::string text);
    /**
     * Read data from the given path and store it into docs, doc_to_token_counts and token_to_doc_counts.
     *
     * @param read_dir   the path to read the data
     */
    void read_data (std::string read_dir);
};

#endif