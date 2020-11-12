# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import nltk
from functools import lru_cache
import string
import argparse
import os

# stop_words = set(nltk.corpus.stopwords.words('english'))
stop_words = {'ourselves', 'hers', 'between', 'yourself', 'but', 'again', 'there', 'about', 'once', 'during', 'out', 'very', 'having', 'with', 'they', 'own', 'an', 'be', 'some', 'for', 'do', 'its', 'yours', 'such', 'into', 'of', 'most', 'itself', 'other', 'off', 'is', 's', 'am', 'or', 'who', 'as', 'from', 'him', 'each', 'the', 'themselves', 'until', 'below', 'are', 'we', 'these', 'your', 'his', 'through', 'don', 'nor', 'me', 'were', 'her', 'more', 'himself', 'this', 'down', 'should', 'our', 'their', 'while', 'above', 'both', 'up', 'to', 'ours', 'had', 'she', 'all', 'no', 'when', 'at', 'any', 'before', 'them', 'same', 'and', 'been', 'have', 'in', 'will', 'on', 'does', 'yourselves', 'then', 'that', 'because', 'what', 'over', 'why', 'so', 'can', 'did', 'not', 'now', 'under', 'he', 'you', 'herself', 'has', 'just', 'where', 'too', 'only', 'myself', 'which', 'those', 'i', 'after', 'few', 'whom', 't', 'being', 'if', 'theirs', 'my', 'against', 'a', 'by', 'doing', 'it', 'how', 'further', 'was', 'here', 'than'}
class Preprocessor:

    def __init__(self):
        self.stem = lru_cache(maxsize=10000)(nltk.stem.snowball.SnowballStemmer("english").stem)
        #Whitespace tokenizer
#        self.tokenize = nltk.tokenize.WhitespaceTokenizer().tokenize
        # Word tokenizer
        self.tokenize = nltk.tokenize.word_tokenize

    def __call__(self, text):
        # normalisation: punctuation removal
        text = text.translate(str.maketrans('', '', string.punctuation))
        # tokenization
        tokens = nltk.WhitespaceTokenizer().tokenize(text)
        # normalisation: stopwords removal
        tokens = [token for token in tokens if token.lower() not in stop_words]
        # Lemmatization
#        tokens = [lemma.lemmatize(token, pos = "v") for token in tokens]
#        tokens = [lemma.lemmatize(token, pos = "n") for token in tokens]
        # stemming
        tokens = [self.stem(token) for token in tokens if token.lower() not in stop_words]
        return tokens
    
parser = argparse.ArgumentParser(description='Tokenize and stem the query')
parser.add_argument('--input', default=None, help='the file containing the query to be tokenized and stemmed.')
parser.add_argument('--output', default=None, help='the file to store query after tokenization and stemming.')
args = parser.parse_args()
input_path = os.path.join(args.input)
output_path = os.path.join(args.output)
input_file = open(input_path, 'r', encoding='utf-8')
output_file = open(output_path, 'w', encoding='utf-8')
preprocessor = Preprocessor()

line = input_file.readline()
while line:
    if not line.startswith('.'):
        line = " ".join(preprocessor(line)) + "\n"
    output_file.write(line)
    line = input_file.readline()

input_file.close()
output_file.close()