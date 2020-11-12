#ifndef INFO_RETRIEVAL_SYSTEM_HPP
#define INFO_RETRIEVAL_SYSTEM_HPP

#include <vector>
#include <string>
#include "hash_table.hpp"
#include "data_loader.hpp"

DataLoader dataset;
HashTable<int, int> doc_to_score;
vector<int> results;
vector<vector<int>> clusters;

HashTable<int, int> retrieve (HashTable<int, HashTable<std::string, int>> doc_to_token_counts, HashTable<std::string, int>);
vector<int> getResult (HashTable<int, int> doc_to_score, int num);
vector<string> schedule (vector<int> results, int time_limit, HashTable<int, struct doc> docs);
vector<vector<int>> clustering (HashTable<int, HashTable<std::string, int>> doc_to_token_counts);

#endif