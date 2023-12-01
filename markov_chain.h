#ifndef _MARKOV_CHAIN_H_
#define _MARKOV_CHAIN_H_

#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate"\
            "new memory\n"
#define FILE_ERROR_MASSAGE "ERROR: file is not valid.\n"
#define ARG_ERROR_MASSAGE "USAGE: must be 3 or 4 arguments.\n"

#define SUCCESS 0
#define FAIL 1
#define DOT_ASCII 46
#define MAX_WORDS 20


typedef struct MarkovChain{
    LinkedList *database;
} MarkovChain;

typedef struct MarkovNode{
    char * data;
    struct MarkovNodeFrequency *frequencies_list;
    int len_frequency ;
} MarkovNode;

typedef struct MarkovNodeFrequency{
    MarkovNode * markov_node;
    int frequency;
}MarkovNodeFrequency;



/**
 * If data_ptr in markov_chain, return it's node. Otherwise, create new
 * node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database,
 * returns NULL in case of memory allocation failure.
 */
Node* add_to_database(MarkovChain *markov_chain, char *data_ptr);

/**
* Check if data_ptr is in database. If so, return the markov_node wrapping it
 *  in the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr);

/**
 * Add the second markov_node to the frequency list of the first markov_node.
 * If already in list, update it's occurrence frequency value.
 * @param first_node
 * @param second_node
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */

bool add_node_to_frequencies_list(MarkovNode *first_node, MarkovNode
*second_node);

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_database(MarkovChain ** ptr_chain);

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain
 * @return
 */
MarkovNode* get_first_random_node(MarkovChain *markov_chain);

/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr);

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with,
 *                   if NULL- choose a random markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_tweet(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length);


#endif /* _MARKOV_CHAIN_H_ */
