#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "hash_table.hpp"
#include "data_loader.hpp"
// #include "info_retrieval_system.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

// DataLoader dataset;
// HashTable<int, int> doc_to_score;
vector<int> results;
vector<vector<int>> clusters;

/**
 * Retrieve relevant documents based on the query and field, and assign each document a score.
 *
 * @param doc_to_token_counts
 * @param query
 * @param field
 * @return hash table <id, score>
 */
HashTable<int, int> retrieve (HashTable<int, HashTable<std::string, int>> doc_to_token_counts, HashTable<std::string, int> query, string field);

/**
 * Retrieve relevant documents based on the query and field, and assign each document a score.
 *
 * @param doc_to_score
 * @param num the number of document wanted
 * @return a vector of doc id
 */
vector<int> getResult (HashTable<int, int> doc_to_score, int num);

/**
 * schedule the reading list based on the documents returned in a given time limit
 *
 * @param results from getResult
 * @param time limit
 * @param docs
 * @return a vector of doc title
 */
vector<string> schedule (vector<int> results, int time_limit, HashTable<int, struct doc> docs);

/**
 * Cluster the documents into a given number of clusters based on the similarity.
 *
 * @param doc_to_token_counts
 * @param numClusters
 * @return a vector of clusters (vector of doc id)
 */
vector<vector<int>> clustering (HashTable<int, HashTable<std::string, int>> doc_to_token_counts, int numClusters);

int main(int argc, char *argv[]) {
    int opt;
    string input = "";
    string query = "";
    int number = 0;
    int time_limit = 0;
    string argstr;

    while ((opt = getopt(argc, argv, "hi:q:n:t:")) != -1) {
        switch (opt) {
            case 'h':
                cout<<"usage: python info_retrieval_system.py [-h] -i INPUT -q QUERY -n NUMBER [-t TIME_LIMIT] [-f FIELD] [-c CLUSTERS]\n"
                    <<"arguments: \n"
                    <<"\t -h \t\t show this help message and exit.\n"
                    <<"\t -i INPUT \t\t of type string.the read-in directory of the dataset.\n"
                    <<"\t -q QUERY \t\t of type string.the query to retrieve documents from the dataset.\n"
                    <<"\t -n NUMBER \t\t of type int.the max number of results to be returned.\n"
                    <<"\t -t TIME_LIMIT \t\t of type int. If given, the scheduler will be used to return the most relevant results which can be read within the time limit in minutes (200 words/min).\n"
                    << endl;
                std::exit(EXIT_SUCCESS);
                break;
            case 'i':
                input = optarg;
                if (input.empty()){
                    fprintf(stderr, "Error: Empty input. Use 'python info_retrieval_system.py -h' to see help.");
                    std::exit(EXIT_FAILURE);
                }
                // cout << "Reading from: " << input << endl;
                continue;
            case 'q':
                query = optarg;
                if (query.empty()){
                    fprintf(stderr, "Error: Empty query. Use 'python info_retrieval_system.py -h' to see help.");
                    std::exit(EXIT_FAILURE);
                }
                // cout << "Query is: " << query << endl;
                continue;
            case 'n':
                argstr = optarg;
                number = stoi(argstr);
                if (number == 0){
                    fprintf(stderr, "Error: Number of results need to be larger than 0. Use 'python info_retrieval_system.py -h' to see help.");
                    std::exit(EXIT_FAILURE);
                }
                // cout << "Number is: " << number << endl;
                continue;
            case 't':
                argstr = optarg;
                time_limit = stoi(argstr);
                continue;
            default: /* '?' */
                fprintf(stderr, "Error: Unknown input. Use 'python info_retrieval_system.py -h' to see help.");
                std::exit(EXIT_FAILURE);
            }
        }
        if (input.empty() || query.empty() || number == 0){
            fprintf(stderr, "Error: Missing parameters. Use 'python info_retrieval_system.py -h' to see help.");
            std::exit(EXIT_FAILURE);
        }
        cout << "Input has been read successfully.\n"
                << "Input: " << input << "\n"
                << "Query: " << query << "\n"
                << "Number: " << number << "\n"
                << "Time limit: " << time_limit << "\n";

        ifstream inFile(input);
        string token;
        int id;
        int count = -1;
        vector<int> lens;
        vector<int> ids;
        HashTable<int, HashTable<string, int>*> *doc_to_token_counts = new HashTable<int, HashTable<string, int>*>();
        HashTable<string, HashTable<int, int>*> *token_to_doc_counts = new HashTable<string, HashTable<int, int>*>();
        while (!inFile.eof()){
            inFile >> token;
            if (token == ".I"){
                inFile >> id;
                HashTable<string, int>* token_counts = new HashTable<string, int>();
                doc_to_token_counts->insert(id, token_counts);
                lens.push_back(0);
                ids.push_back(id);
                count ++;
            } else if (token[0] == '.'){
                continue;
            } else {
                HashTable<string, int>* p_token_counts = *(doc_to_token_counts->get(id));
                int *p_counts = p_token_counts->get(token);
                if ( p_counts == NULL){
                    p_token_counts->insert(token, 1);
                } else {
                    *p_counts = *p_counts + 1;
                }

                HashTable<int, int>** pp_doc_counts = token_to_doc_counts->get(token);
                if (pp_doc_counts == NULL){
                    HashTable<int, int>* doc_counts = new HashTable<int, int>();
                    token_to_doc_counts->insert(token, doc_counts);
                    doc_counts->insert(id, 1);
                } else {
                    int *p_counts_doc = (*pp_doc_counts)->get(id);
                    if (p_counts_doc == NULL){
                        (*pp_doc_counts)->insert(id, 1);
                    } else{
                        *p_counts_doc = *p_counts_doc + 1;
                    }
                }
                lens[count] = lens[count]+1;
            }
        }

        cout << "doc num: " << ids.size() << " id: " << ids[0] << endl;
        cout << "len num: " << lens.size() << " len: " << lens[0] << endl;
        cout << "first doc has fetal: " << *((*(doc_to_token_counts->get(1)))->get("fetal")) <<endl;
        cout << "fetal has in doc freq: " << *((*(token_to_doc_counts->get("fetal")))->get(1)) <<endl;

        std::exit(EXIT_SUCCESS);
        
}