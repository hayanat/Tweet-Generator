#include "markov_chain.h"

int in_frequency_list (MarkovNode *first_node, MarkovNode *second_node);
int update_frequency_exist (MarkovNode *first_node, MarkovNode *second_node);
int add_to_list(MarkovNode *first_node, MarkovNode *second_node);
void free_node(Node * current);
int get_random_number(int max_number);


Node *add_to_database (MarkovChain *markov_chain, char *data_ptr)
{
  Node *cur = get_node_from_database(markov_chain, data_ptr);
  if (cur != NULL ){
    return cur;
  }

  int data_len = (int)strlen (data_ptr) + 1;  //  must add \0 to last index
  char *data = calloc (data_len, 1);
  if (data == NULL)
  { return NULL; }
  strcpy (data, data_ptr);

  MarkovNode *markov_node = calloc (1, sizeof (MarkovNode));
  if (markov_node == NULL)
  {
    free (data);
    return NULL;
  }
  markov_node->data = data;
  markov_node->frequencies_list = NULL;
  markov_node->len_frequency = 0;

  if (add (markov_chain->database, markov_node) == FAIL)
  {
    free (data);
    free (markov_node);
    return NULL;
  }
  return markov_chain->database->last;
}


Node *get_node_from_database (MarkovChain *markov_chain, char *data_ptr)
{
  Node * cur = markov_chain->database->first;
  while (cur){
    if (strcmp (cur->data->data, data_ptr) == 0) {
      return cur;
    }
    cur = cur->next;
  }
  return NULL;
}

bool add_node_to_frequencies_list (MarkovNode *first_node, MarkovNode
*second_node)
{
  int x = in_frequency_list (first_node, second_node);
  if (x == SUCCESS)
  { // it already exists before that
    if (update_frequency_exist (first_node, second_node) == SUCCESS)
    { return true; }
  }

  else
  { // it doesn't exist in frequency list - must add and update the number
    if (add_to_list (first_node, second_node) == SUCCESS)
      {return true; }
  }

  return false;
}
/**
 *  the second node doesn't exist in the first node frequency list , in
 *  this function we add it and update the numbers (frequency , len_frequency)
 * @param first_node  the first node
 * @param second_node the second node
 * @return SUCCESS upon success, FAIL otherwise
 */
int add_to_list (MarkovNode *first_node, MarkovNode *second_node){
  int new_n = (first_node->len_frequency) + 1;
  MarkovNodeFrequency * head = first_node->frequencies_list;

  if(head == NULL){   // MarkovNodeFrequency is NULL
    MarkovNodeFrequency *temp = calloc (1,sizeof (MarkovNodeFrequency));
    if(temp == NULL) {
      return FAIL;
    }
    first_node->frequencies_list = temp;
    first_node->frequencies_list->frequency = 1;
    first_node->len_frequency = 1;
    first_node->frequencies_list->markov_node = second_node;
    return SUCCESS;
  }
  else
  {
    MarkovNodeFrequency *temp = realloc (head, sizeof(*temp) * new_n);
    if(temp == NULL) {
      return FAIL;
    }

    first_node->frequencies_list = temp;
    MarkovNodeFrequency * arr = temp;
    first_node->len_frequency += 1;
    arr[new_n -1].markov_node = second_node;
    arr[new_n -1].frequency = 1;
    return SUCCESS;
  }
}

/**
 * checks if the second node exists in the first node frequency list
 * @param first_node the first node we check in it's frequency list
 * @param second_node the second node
 * @return SUCCESS if it exists, FAIL otherwise
 */
int in_frequency_list (MarkovNode *first_node, MarkovNode *second_node) {
  if (first_node->len_frequency == 0) {
    return FAIL;
  }
  int n =first_node->len_frequency;
  MarkovNodeFrequency * cur = first_node->frequencies_list;

  for(int i =0; i < n; i++){
    if (strcmp(cur->markov_node->data, second_node->data) == SUCCESS) {
      return SUCCESS;
    }
    cur ++;
  }
  return FAIL;
}

/**
 * the second node already exists in the first node frequency list, in this
 * function we update len_frequency and frequency
 * @param first_node the first node
 * @param second_node the second node
 * @return SUCCESS upon success , FAIL otherwise.
 */
int update_frequency_exist (MarkovNode *first_node, MarkovNode *second_node){
  int n =first_node->len_frequency;
  MarkovNodeFrequency * cur = first_node->frequencies_list;

  for(int i =0; i < n; i++){
    if (strcmp(cur->markov_node->data, second_node->data) == SUCCESS) {
      cur->frequency ++;
      return SUCCESS;
    }
    cur ++;
  }
  return FAIL;
}



void free_database (MarkovChain **ptr_chain)
{
  if (*ptr_chain == NULL) {
    return;
  }

  if((*ptr_chain)->database == NULL){
    free (*ptr_chain);
    return;
  }

  // now : ((*ptr_chain)->database) != NULL
  if((*ptr_chain)->database->size == 0) {
    free((*ptr_chain)->database);
    free (*ptr_chain);
    return;
    }

  //now : ((*ptr_chain)->database->size > 0)
  Node * cur_node = (*ptr_chain)->database->first;
  (*ptr_chain)->database->first = cur_node->next ;
  free_node(cur_node); // to free the first node

  while ((*ptr_chain)->database->first  != NULL){
    //next_node = cur_node->next;
    cur_node = (*ptr_chain)->database->first;
    (*ptr_chain)->database->first = cur_node->next ;
    free_node(cur_node); // to free the first node

  }
  free((*ptr_chain)->database);
  free (*ptr_chain);

}
/**
 * free node
 * @param current  a pointer to a node
 */
void free_node(Node * current){
  if(current == NULL) { // there is no node
    return;
  }
  // node is not null
  if(current->data == NULL){ // there is no markov node
    free(current); // free node
    return;
  }

  // node is not null and there is a markov node
  if(current->data->len_frequency == 0) {
    // NO FREQUENCY LIST
    free(current->data->data);
    free(current->data);
    free(current);
    return;
  }
  // there is a node and markov node and frequency list
  free(current->data->frequencies_list);
  free (current->data->data);
  free(current->data);
  free(current);
}

/**
 * get random number between 0 and max_number [0, max_number(
 * @param max_number maximal number to return (not including)
 * @return random number
 */
int get_random_number(int max_number){
  return rand() % max_number;
}

MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  if(markov_chain->database == NULL || markov_chain->database->size == 0){
    return NULL;
  }
  while(true)
  {
    int i = 0;
    int num = get_random_number (markov_chain->database->size);
    Node * cur_node = markov_chain->database->first;
    while (cur_node != NULL)
    {
      if (i == num){
        int last_index_cur = (int)strlen (cur_node->data->data) - 1 ;
        if (cur_node->data->data[last_index_cur] == DOT_ASCII)
          { break;}
        return cur_node->data;
      }
      cur_node = cur_node->next;
      i++;
    }
  }
}

MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)
{
  if (state_struct_ptr->frequencies_list == NULL) {
    return NULL;
  }

  int all_sum = 0;
  MarkovNodeFrequency * pass = state_struct_ptr->frequencies_list ;
  int len = state_struct_ptr->len_frequency;
  for(int j = 0 ; j < len; j++){
    all_sum += pass->frequency;
    pass ++ ;
  }
  int random = get_random_number (all_sum);
  MarkovNodeFrequency * cur = state_struct_ptr->frequencies_list ;
  int cumulative_count = 0;

  for(int k = 0 ; k < random; k++)
  {
    cumulative_count += cur->frequency;
    if(cumulative_count > random){
      break;
    }
    cur ++;
    }

  return cur->markov_node;
}

void generate_tweet (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
  if (!(markov_chain->database) || !(markov_chain->database->first)){
    return;
  }
  int i = 0; // i is maximum 20
  if(first_node == NULL){ // IT IS NULL >> MUST PICK A START
    first_node = get_first_random_node (markov_chain); }

  MarkovNode * cur_m_node = first_node;
  printf ("%s ", cur_m_node->data); // print the first word
  ++i;

  cur_m_node = get_next_random_node (cur_m_node); // the second node
  int last_ind = (int)strlen (cur_m_node->data) - 1;

  while(cur_m_node->data[last_ind] != DOT_ASCII){
    if (i == (max_length - 1)) {
      printf ("%s", cur_m_node->data); // print the last word
      return;
    }
    printf ("%s ", cur_m_node->data); // print the current word
    ++i;
    cur_m_node = get_next_random_node (cur_m_node);
    if(cur_m_node == NULL) {return;} // must check for extra space
    last_ind = (int)strlen (cur_m_node->data) - 1;
  }
  printf ("%s", cur_m_node->data); // print the last word ( i<20)
}
