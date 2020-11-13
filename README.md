# Document retrieval system

## Description         
The document retrieval system works on the corpus in the defined structure (-i INPUT). It can search through documents by the query (-q QUERY). The top n results will be returned (-n NUMBER). It can also schedule a most valuable reading list based on the documents returned if a time limit is given (-t TIME_LIMIT).

The sample datasets is given as an example (med_all.txt) (Medline dataset from http://ir.dcs.gla.ac.uk/resources/test_collections/medl/). The file format shoud be space seperated. ".I" indicates the next word is doc id. ".W" indicates doc contents follow after it. Original dataset also privides queries (med_quiry.txt), in the same format, and ground truth (MED.REL) (which docs are relate to the quiry). Ground truth file is in a format "query_id 0 relevant_doc_id 1".

Input files and queries need preprocessing. The provided med_all.txt and med_query.txt have been preprocessed by preprocess.py. Using Python is because Python has NLP tools more easier to use to do tokenization, stemming and so on. If you want to run your own query or dataset. Please format them the same as provided datasets and then run preprocess.py on them. After that you can use the preprocessed text as inputs to the system.

## Usage
.\main [-h] -i INPUT -q QUERY -n NUMBER [-t TIME_LIMIT]


         -h                      show this help message and exit.
         -i INPUT                of type string.the read-in directory of the dataset.
         -q QUERY                of type string.the query to retrieve documents from the dataset.
         -n NUMBER               of type int.the max number of results to be returned.
         -t TIME_LIMIT           of type int. If given, the scheduler will be used to return the most relevant results which can be read within the time limit in swconds (3 words/s).

## Commands
To compile:

g++ .\doc_retrieval_system.cpp .\heap_hash_dp.cpp -o main

To run:

.\main [paramters]

To compile test codes:

 g++ .\test_hash_table.cpp .\heap_hash_dp.cpp -o test_hash_table 

 .\test_hash_table test-f1-[i].txt

g++ .\test_heap.cpp .\heap_hash_dp.cpp -o test_heap 

.\test_heap test-f2-[i].txt

g++ .\test_dp.cpp .\heap_hash_dp.cpp -o test_dp 

.\test_dp test-f3-[i].txt

To run preprocessing.py:

python preprocess.py --input input_file_name --output output_file_name

# Testing
To test 3 functionalities, I provide one test file for each to check if it passes the test cases.

To test the program, you can manually pick on query from med_query.txt and pass it to the system. Then, you can manually check with MED_REL how many results are overlapped. After test, for 15 returned results, the average precision is around 43% (This med database is actually quite academic and hard).

# Example
```
>> g++ .\doc_retrieval_system.cpp .\heap_hash_dp.cpp -o main
>> .\main  -i med_all.txt -n 10 -q "crystallin len vertebr includ human" -t 60
Input has been read successfully.
Input: med_all.txt
Query: crystallin len vertebr includ human
Number: 10
Time limit: 60
finish reading in dataset, 1033 in total
finish reading in query
start searching...
Searching result:
1. 500
2. 72
3. 181
4. 180
5. 965
6. 168
7. 509
8. 206
9. 360
10. 138
reading list: 360 72 500
(sum of relevance score: 37)
>> g++ .\test_hash_table.cpp .\heap_hash_dp.cpp -o test_hash_table
>> .\test_hash_table test-f1-5.txt
insert time: 0
insert time per key: 0
search time: 15587500
search time per key: 825
Every key is inserted and found successfully
```



