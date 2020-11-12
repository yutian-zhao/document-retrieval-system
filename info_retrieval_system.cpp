#include <iostream>
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
    string field = "content";
    int number = 0;
    int time_limit = 0;
    int clusters = 0;
    string argstr;

    while ((opt = getopt(argc, argv, "hi:q:n:t:f:c:")) != -1) {
        switch (opt) {
            case 'h':
                cout<<"usage: python info_retrieval_system.py [-h] -i INPUT -q QUERY -n NUMBER [-t TIME_LIMIT] [-f FIELD] [-c CLUSTERS]\n"
                    <<"arguments: \n"
                    <<"\t -h \t\t show this help message and exit.\n"
                    <<"\t -i INPUT \t\t of type string.the read-in directory of the dataset.\n"
                    <<"\t -q QUERY \t\t of type string.the query to retrieve documents from the dataset.\n"
                    <<"\t -n NUMBER \t\t of type int.the max number of results to be returned.\n"
                    <<"\t -t TIME_LIMIT \t\t of type int. If given, the scheduler will be used to return the most relevant results which can be read within the time limit in minutes (200 words/min).\n"
                    <<"\t -f FIELD \t\t of type int.If given, the query will be run on a specific field. [title, content, author] (default: content)\n"
                    <<"\t -c CLUSTERS \t\t of type int. If given, the dataset will be grouped into the corresponding number of clusters."
                    << endl;
                exit(EXIT_SUCCESS);
                break;
            case 'i':
                input = optarg;
                if (input.empty()){
                    fprintf(stderr, "Error: Empty input. Use 'python info_retrieval_system.py -h' to see help.");
                    exit(EXIT_FAILURE);
                }
                // cout << "Reading from: " << input << endl;
                continue;
            case 'q':
                query = optarg;
                if (query.empty()){
                    fprintf(stderr, "Error: Empty query. Use 'python info_retrieval_system.py -h' to see help.");
                    exit(EXIT_FAILURE);
                }
                // cout << "Query is: " << query << endl;
                continue;
            case 'n':
                argstr = optarg;
                number = stoi(argstr);
                if (number == 0){
                    fprintf(stderr, "Error: Number of results need to be larger than 0. Use 'python info_retrieval_system.py -h' to see help.");
                    exit(EXIT_FAILURE);
                }
                // cout << "Number is: " << number << endl;
                continue;
            case 't':
                argstr = optarg;
                time_limit = stoi(argstr);
                continue;
            case 'f':
                field = optarg;
                if (field != "content" && field != "title" && field != "author"){
                    fprintf(stderr, "Error: Wrong field. Use 'python info_retrieval_system.py -h' to see help.");
                    exit(EXIT_FAILURE);
                }
                continue;
            case 'c':
                argstr = optarg;
                clusters = stoi(argstr);
                continue;
            default: /* '?' */
                fprintf(stderr, "Error: Unknown input. Use 'python info_retrieval_system.py -h' to see help.");
                exit(EXIT_FAILURE);
            }
        }
        if (input.empty() || query.empty() || number == 0){
            fprintf(stderr, "Error: Missing parameters. Use 'python info_retrieval_system.py -h' to see help.");
            exit(EXIT_FAILURE);
        }
        cout << "Input has been read successfully.\n"
                << "Input: " << input << "\n"
                << "Query: " << query << "\n"
                << "Number: " << number << "\n"
                << "Time limit: " << time_limit << "\n"
                << "Field: " << field << "\n"
                << "Clusters: " << clusters << endl;
        exit(EXIT_SUCCESS);
        
}