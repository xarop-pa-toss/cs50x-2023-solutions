// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <cs50.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

typedef struct hashtable
{
    char key;
    char *next;
}
hashtable;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Int to hold dictionary size
int dictionarySize = 0;

// Hash table with N buckets
node *table[N];


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Get hashtable position and get a node that goes through the list by pointing to each element one by one.
    // Using a made up node is necessary to not mess with the linked list structure. It can just point to the next node until it finds whatever.
    int hashIndex = hash(word);
    node *temp = table[hashIndex];

    while (temp != NULL)
    {
        if (strcasecmp(word, temp->word) == 0)
        {
            return true;
        }
        // If it doesnt find the word on a node, assigns itself to the next node.
        temp = temp->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // Tried to implement a folding hash function
    //https://research.cs.vt.edu/AVresearch/hashing/strings.php and https://www.geeksforgeeks.org/folding-method-in-hashing/
    int asciiSum = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        /*if (word[i] == '\'')
        {
            continue;
        } */
        asciiSum += tolower(word[i]);
    }
    return asciiSum % N;
}


// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    char word[LENGTH];

    // Create array of head nodes. One node for each letter of the alphabet
    // node *head[N] = { NULL };

    // MAIN LOGIC LOOP
    while (fscanf(dict, "%s", word) != EOF)
    {
        // Create new node with allocated memory for itself
        node *wordNode = NULL;
        wordNode = malloc(sizeof(node));
        if (wordNode == NULL)
        {
            return false;
        }

        // Copy word into the new node
        strcpy(wordNode->word, word);
        // Get index to use in table[]
        int index = hash(word);

        // Push into beginning of linked list defined by hash() by pointing the new node to the current head, and the dead node to the new (now first) node.
        wordNode->next = table[index];
        table[index] = wordNode;
        //wordNode->next = head[index];
        //head[index] = wordNode;    printf("Dictionary \n");

        dictionarySize++;
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return dictionarySize;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // Go through all heads and free nodes until it returns NULL, meaning the linked list is empty.
    // As before, use a cursor node that just points at whatevers next and assigns it to a temp node.
    // Much like pushing into the linked list but in reverse
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        while (cursor != NULL)
        {
            // temp becomes = to cursor (but IS NOT cursor). Doing this so we don't lose connection with the linked list
            node *temp = cursor;
            // cursor points to the next element
            cursor = cursor->next;
            // now that the connection is secure, we can clean the node from the list.
            free(temp);
        }
        // Was battling forever with a single block showing up in valgrind from wordNode. Had to look it up. Sorry :(
        // Tried freeing all sorts of stuff
        if (cursor == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false;
}