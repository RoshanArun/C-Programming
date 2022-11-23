// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }


    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // TODO
        //malloc space for new node
        node *new_node = malloc(sizeof(node));

        //see if new node was created
        if(new_node == NULL){
            unload();
            return false;
        }
        //put node into the new node
        strcpy(new_node->word, word);

        //store the word into a variable for later
        int bucket = hash(word);

        if(hashtable[bucket] == NULL){
            //point bucket to the first node
            hashtable[bucket] = new_node;
            //set to null to see where end of list is
            new_node->next = NULL;
        }
        else{
            //points the next pointer to the new node, where the bucket is pointing to
            new_node->next = hashtable[bucket];
            hashtable[bucket] = new_node;
        }
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    unsigned int counter = 0;
    node *current = NULL;

    for(int i = 0; i < N; i++){
        //point current to the same thing the current bucket points to
        current = hashtable[i];
        //use a while loop that goes down linked list and counts number of words
        while(current != NULL){
            counter = counter + 1;
            current = current->next;
        }
    }
    return counter;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    int bucket = hash(word);
    node *find = NULL;
    find = hashtable[bucket];

    while(find != NULL){
        if(strcasecmp(find->word, word) == 0){
            return true;
        }
        else{
            find = find->next;
        }
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    //loops through all the buckets
    for(int i = 0; i < N; i++){
        //point what the bucket points to
        node *cursor = hashtable[i];
        //while the cursor equals null
        while(cursor != NULL){
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}
