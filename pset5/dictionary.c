// Implements a dictionary's functionality

/* I referred to this article during selection of hush function
https://softwareengineering.stackexchange.com/questions/49550/which-hashing-algorithm-is-best-for-uniqueness-and-speed
*/
//==============================================================================
/* INSERT  - used on load() function
1.  Dynamically allocate space for new_node
2.  Check for memory error
3.  If specific hash_table is empty, then point hash_table pointer to new_node
4.  If #3 is false - Insert the new_node at the Beginning of linked list
5.  Return pointer to new_node
*/
//==============================================================================
/* SEARCH - to be used in check() funtion
1.  Create a cursor ptr pointing the the head of hash_table
2.  If the current node value == cursor ptr, return success
3.  If not, set the cursor ptr to the next pointer in the linked list and got back to #2
4.  If reached end of linked list of selected hash_table, return failure
*/
//==============================================================================
/* FREE MEMORY - used in unload()
1.  Point cursor ptr where head_node is pointing
2.  Point temp ptr where cursor is pointing
3.  Move cursor ptr on next
4.  Free memory on temp ptr and go to step #2
*/
//==============================================================================

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

// #include "sdbm_hash.h"

#define HASH_TABLE_SIZE 80000

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;



// Hash table with HASH_TABLE_SIZE number of buckets
node *header_node[HASH_TABLE_SIZE];

int word_num = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int length = strlen(word);
    char buffer[length + 1];
    for(int i = 0; i < length + 1; i++)
    {
        buffer[i] = tolower(word[i]);
    }

    //printf("word[] = %s ", buffer);
    unsigned int index = SDBMHash(buffer, length)%HASH_TABLE_SIZE;
    //printf(" ->Hash = %u\n", index);

    //SEARCH #1
    node *cursor = header_node[index];

    while (cursor != NULL)
    {
        //  SEARCH #2
        if (strcmp(cursor->word, buffer) == 0)
        {
            return true;
        }
        // SEARCH #3
        cursor = cursor->next;
    }
    // SEARCH #4
    return false;
}

// Hashes word to a number
unsigned int SDBMHash(const char* str, unsigned int length)
{
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = (*str) + (hash << 6) + (hash << 16) - hash;
   }

   return hash;
}

// #1 - Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //fill our hashtable with NULL  values. #HEADER ptr
    for (int i = 0; i > HASH_TABLE_SIZE; i++)
    {
        header_node[i] = NULL;
    }

    FILE *r_file = fopen(dictionary, "r");
    if (r_file == NULL) //check if file was opened
    {
        fprintf(stderr, "Couldn't OPEN file %s\n", dictionary);
        return 1;
    }

    //length including '/0'
    char buffer[LENGTH+1];
    int l = LENGTH +1;

    while (fscanf(r_file, "%45s", buffer) != EOF)
    {
        unsigned int ll = strlen(buffer);
        unsigned int index;
        //printf("word[] = %i and length = %lu  ", buffer[ll], strlen(buffer));


        index = SDBMHash(buffer, ll)%HASH_TABLE_SIZE;
        //printf(" ->Hash = %u\n", index);


        //  INSERT #1
        node *new_node = malloc(sizeof(node));
        //  INSERT #2
        if (new_node != NULL)
        {
            strcpy(new_node -> word, buffer); //copy extracted word to placeholder
            new_node -> next = NULL; //copy NULL to adress placeholder

        }
        else
        {
            printf("Could not allocate memory");
            return 2;
        }
        //  INSERT #3
        if (header_node[index] == NULL)
        {
            header_node[index] = new_node;
        }
        //  INSERT #4
        else
        {
            new_node -> next = header_node[index];
        //  INSERT #5
            header_node[index] = new_node;
        }

        word_num ++;
    }
    //if (feof(r_file))
    //printf("\n Reached to the end of file with lines: %i\n", word_num);

    fclose(r_file);
    return true;

}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_num;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        node *cursor = header_node[i];
        if (header_node[i] != NULL)
        {
            continue;
        }
        while (cursor != NULL)
        {
            node *tmp = header_node[i];
            cursor = cursor -> next;
            free(tmp);
        }
    }
    return true;
}
