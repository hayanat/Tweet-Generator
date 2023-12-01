# Tweet Generator

Using a Computer Program to Generate New Sentences/Tweets Using an Existing Database of Tweets via Markov Chains
The program operates in three stages:

1. Input Stage:
The program will take as input a text file containing a large database of existing tweets, from which it will construct the new tweets.

2. Learning Stage:
The program will read the text file, store, and process the sentences and words into a data structure. For each word, it will also calculate the frequency of the words that appear after it in the file.

3. Output Stage - Generating Tweets:
The program will use the data structures created and the calculated frequencies from the input file to generate tweets in a probabilistic manner.