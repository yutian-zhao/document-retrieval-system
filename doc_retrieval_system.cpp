#include "heap_hash_dp.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <cmath>

using namespace std;

int main(int argc, char *argv[]) {

    // interative interface and get inputs
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

        // store dataset into hashtable
        ifstream inFile(input);
        string token;
        int id;
        int count = -1;  
        vector<int> lens;  // the length of each doc
        vector<int> ids;  // the id of each doc
        // {doc:{token:counts}}
        HashTable<int, HashTable<string, int>*> *doc_to_token_counts = new HashTable<int, HashTable<string, int>*>();
        // {token:{doc:counts}}
        HashTable<string, HashTable<int, int>*> *token_to_doc_counts = new HashTable<string, HashTable<int, int>*>(11000);
        while (!inFile.eof()){
            inFile >> token;
            if (token == ".I"){
                // id
                inFile >> id;
                HashTable<string, int>* token_counts = new HashTable<string, int>();
                doc_to_token_counts->insert(id, token_counts);
                lens.push_back(0);
                ids.push_back(id);
                count ++;
            } else if (token[0] == '.'){
                continue;
            } else {
                // token
                // add to doc_to_token_counts
                HashTable<string, int>* p_token_counts = *(doc_to_token_counts->get(id));
                int *p_counts = p_token_counts->get(token);
                if ( p_counts == NULL){
                    p_token_counts->insert(token, 1);
                } else {
                    *p_counts = *p_counts + 1;
                }

                // add to token_to_doc_counts
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
        inFile.close();
        cout << "finish reading in dataset, " << doc_to_token_counts->numElements << " in total" << endl;


        // store query in {token:counts}
        HashTable<string, int>* query_terms = new HashTable<string, int>(50);
        vector<string> tokens;
        istringstream iss(query);
        // string token;
        while(iss >> token) {
            int* p_query_term_freq = query_terms->get(token);
            if (p_query_term_freq == NULL){
                query_terms->insert(token, 1);
            } else{
                *p_query_term_freq = *p_query_term_freq + 1;
            }
            tokens.push_back(token);
        }
        cout << "finish reading in query"  << endl;


        cout << "start searching..." << endl;
        // store relevance score for corresponding doc
        vector<int> scores_vec(ids.size(), 0); 
        // used to build heap for sorting
        KVPair<float, int> **scores = new KVPair<float, int> *[ids.size()]();
        for (int i = 0; i<ids.size(); i++){
            scores[i] = new KVPair<float, int>(0, i); // value is index
        }

        // avgdl average doc length
        int sum_lens = 0;
        for (int i=0; i<lens.size();i++){
            sum_lens = sum_lens + lens[i];
        }
        float avgdl = sum_lens/lens.size();
        int k = 2;
        int b = 0.75;

        // use BM25 similarity as relevance score, see https://en.wikipedia.org/wiki/Okapi_BM25 for reference
        for (int i=0; i<tokens.size();i++){
            // nq # number of documents containing query token
            int nq = (*(token_to_doc_counts->get(tokens[i])))->numElements;
            // # idf for query token
            float idfq = log(((lens.size()-nq+0.5)/(nq+0.5))+1);
            for (int j=0; j<ids.size(); j++){
                int* tf = (*(token_to_doc_counts->get(tokens[i])))->get(ids[j]);
                if (tf != NULL){
                    float temp = idfq * (((*tf)*(k+1))/((*tf)+k*(1-b+b*((*(doc_to_token_counts->get(ids[j])))->numElements)/avgdl)));
                    if (temp < 0){
                        temp = 0;
                    }
                    scores[j]->key = scores[j]->key + temp;
                    scores_vec[j] = scores_vec[j] + temp;
                }
            }
        }


        // Heapsort for top k (number) results
        Heap<float, int> *score_heap =  new Heap<float, int>(scores, ids.size());
        vector<int> top_k;
        cout << "Searching result: " << endl;
        for (int i=0; i<number; i++){
            top_k.push_back(score_heap->extractMax());
            cout << i+1 << ". " << ids[top_k[i]] << endl;
        }


        // Schedule
        if (time_limit > 0){
            // create time and value vector for top k results
            vector<int> times;
            vector<float> values;
            for (int i=0; i<top_k.size();i++){
                times.push_back(std::ceil(lens[top_k[i]]/3));
                values.push_back(scores_vec[top_k[i]]);
            }

            // initialize kc
            vector<vector<float>> kc(number+1, vector<float>(time_limit+1));
            for (int j=0; j<= time_limit; j++){
                kc[0][j] = 0; 
            }

            // dp
            int sum = bottom_up(time_limit, number, kc, times, values);
            vector<int> results = extract(time_limit, number, kc, times, values);
            cout << "reading list:";
            for (int i=0; i<results.size(); i++){
                cout << " " << ids[top_k[results[i]]];
            }
            cout << endl;
            cout << "(sum of relevance score: " << sum << ")" << endl;
        }


        delete doc_to_token_counts;
        delete token_to_doc_counts;
        delete query_terms;
        delete score_heap;

        std::exit(EXIT_SUCCESS);
        
}