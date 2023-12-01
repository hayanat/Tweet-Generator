
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "markov_chain.h"

#define DECIMAL_BASE 10
#define READ_ALL (-1)
#define MAX_SENTENCE 1001



int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain);


/**
 * the main function gets arguments and prints tweets
 * @param argc  number of argument - including file name
 * @param argv array of arguments
 * @return EXIT_SUCCESS if everything worked ,EXIT_FAILURE otherwise
 */
int main (int argc, char *argv[])
{
  int arg_num = argc - 1; // arguments starts from index 1
  if ((arg_num != 3) && (arg_num != 4))
  { printf("%s", ARG_ERROR_MASSAGE);
    return EXIT_FAILURE; }
  unsigned int seed = strtol (argv[1],NULL,DECIMAL_BASE);
  srand (seed);
  int tweet_num = (int) strtol (argv[2],NULL,DECIMAL_BASE);
  int words_to_read = READ_ALL;
  if (arg_num == 4) {
    words_to_read = (int) strtol (argv[4],NULL,DECIMAL_BASE);}

  LinkedList *database = calloc (1, sizeof (LinkedList));
  if(database == NULL){
    printf ("%s", ALLOCATION_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }
  MarkovChain * markov_chain = calloc (1,sizeof (MarkovChain));
  if(markov_chain == NULL){
    free(database);
    printf ("%s", ALLOCATION_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }
  *markov_chain = (MarkovChain) {database};
  FILE *in_file = fopen (argv[3], "r");
  if (in_file == NULL)
  { // if input file doesn't exist.
    free_database (&markov_chain);
    printf ("%s", FILE_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }
  if (fill_database (in_file, words_to_read, markov_chain) == FAIL){
    free_database (&markov_chain);
    fclose(in_file);
    printf ("%s", ALLOCATION_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }
  fclose (in_file);
  for (int i = 1; i <= tweet_num; i++){
    printf ("Tweet %d: ", i);
    generate_tweet (markov_chain, NULL, MAX_WORDS);
    printf ("\n");
  }
  free_database (&markov_chain);
  markov_chain = NULL;
  return EXIT_SUCCESS;
}


/**
 * this function fills the data base
 * @param fp  file to read
 * @param words_to_read number of words to read
 * @param markov_chain pointer to a markov chain
 * @return  0 upon SUCCESS , 1 otherwise.
 */
int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain){
  char line [MAX_SENTENCE] = {0};
  int i =0;
  bool limit = true;
  while (fgets (line, MAX_SENTENCE, fp) && limit)
  {
    char *word = strtok(line, " \n");
    while (word != NULL) {
      if (strlen(word) > 0 && strcmp(word, " ") != 0)
      { // check if word is not empty or just spaces
        if(add_to_database(markov_chain, word) == NULL) {
          return FAIL;
        }
        i++;
        if (words_to_read != READ_ALL)
          {limit = (i<words_to_read);}
      }
      if(i==words_to_read){ break;}

      char *next = strtok(NULL, " \n\r");
      int last_index_word = (int) strlen (word) - 1 ;
      if (next == NULL || word[last_index_word] == DOT_ASCII) {
        word = next;
        continue;
      }
      // add next to database
      Node *node_next = add_to_database(markov_chain, next);
      if(node_next == NULL) {
        return FAIL;
      }
      Node *node_cur= get_node_from_database (markov_chain,word);
      if(add_node_to_frequencies_list (node_cur->data,node_next->data) ==
      false){
        return FAIL;
      }
      word = next;  // get next word
      }
  }
  return SUCCESS;
}