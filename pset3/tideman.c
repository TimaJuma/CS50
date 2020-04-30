/*
this program is preliminary
1) sorting algorithm is bubble sort since not many items to be sorted. But still, it will be optimized soon
2) I have to revise my understanding of locking the edge of the graph
3) Code contains some templates to print preliminary results. to be removed from final version.
4) So far, this code didn't pass tests for sort() and lock()
*/

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#define MAX 9                       // maximum # of Voters accepted

typedef struct                      // #3
{
    int winner;
    int loser;
}
pair;
pair pairs[MAX * (MAX / 2)];        // #3 - maximum pairs posibility n*n/2

int win_strength[MAX * (MAX / 2)];  // #3.1 - corresponding array to have victory strength value


int preferences[MAX][MAX];          // #2
int candidate_count;                // main
string candidates[MAX];             // main
int pair_count;                     // #3
bool locked[MAX][MAX];              // #5

//function to be used
bool vote(int rank, string name, int ranks[]); // #1
void record_preferences(int ranks[]);          // #2
void add_pairs(void);                          // #3
void victory_strength(void);                   // #3.1
void swap(int *xp, int *yp);                   // #4.1
void sort_pairs(void);                         // #4.2
void lock_pairs(void);                         // #5
void print_winner(void);                       // #6




// ====================MAIN====================================================

int main(int argc, string argv[])
{
    // preliminary check for invalid inputs
    if (argc < 2)
    {
        printf("Please enter at least one candidate in format if ./tideman...");
        return 1;
    }
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of voters that can vote is %i\n", MAX);
        return 2;
    }

    // ============================================
    // initialize arrays

    // array of candidates fill with names
    for (int ii = 0; ii < candidate_count; ii++)
    {
        candidates[ii] = argv[ii + 1];
    }

    // set array of preferences filled with 0 votes, it will be incremented with ranks[] at each vote iteraton
    // set elements of locked - 2D array with false values
    for (int k = 0; k < candidate_count; k++)
    {
        for (int l = 0; l < candidate_count; l++)
        {
            preferences[k][l] = 0;
            locked[k][l] = false;
        }
    }


    int voter_count = get_int("Number of voters: \n");
    // iterate over number of votes given
    for (int i = 0; i < voter_count; i++)
    {
        // create ranks that shall be new at each vote iteration
        int ranks[candidate_count];
        printf("Vote # %i\n", i + 1);

        // iterate to ask name to allocate with each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);
            //#1 - pass values to VOTE function and update ranks[i] array if name is found in candidates[candidate_count] array
            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n"); // case when name entered wrongly
                return 3;
            }
        }
        printf("\n");

        // #2   - at each vote iteration passes ranks[i] array to update preferences
        record_preferences(ranks);
    }

    /*
    // result cheking template
    for (int z=0; z<candidate_count; z++)
    {
        for (int zz=0; zz<candidate_count; zz++)
        {
            printf("pref [%i][%i] = %i ", z, zz, preferences[z][zz]);
        }
        printf("\n");
    }
    */


    // #3 - after prefernces[i][j] are finalized pairs can be updated
    pair_count = 0;
    add_pairs();

    /*
    //template to check winner/looser pairs results
    printf("pairs = %i\n", pair_count);
    for (int y = 0; y < pair_count; y++)
    {
        printf("w%i = cand[%i]\n",y, pairs[y].winner);
        printf("l%i = cand[%i]\n",y, pairs[y].loser);
        printf("\n");
    } */

    // #3.1 - after prefernces[i][j] are finalized pairs can be updated
    victory_strength();

    /*
    // template to check strengness winner values
    printf("win_strength: ");
    for (int x = 0; x < pair_count; x++)
    {
        printf("  %i", win_strength[x]);
    }
    printf("\n");
    */


    //#4
    sort_pairs();
    /*
    printf(" new win_strength: ");
    for (int xx = 0; xx < pair_count; xx++)
    {
        printf("  %i", win_strength[xx]);
    }
    printf("\n");


    //template to check winner/looser pairs results
    printf("sorted pair");
    for (int yy = 0; yy < pair_count; yy++)
    {
        printf("w%i = cand[%i]\n",yy, pairs[yy].winner);
        printf("l%i = cand[%i]\n",yy, pairs[yy].loser);
        printf("\n");
    }
    */


    // #5 - lock pairs
    lock_pairs();

    /*
    // result cheking template
    for (int v = 0; v < candidate_count; v++)
    {
        for (int vv = 0; vv <candidate_count; vv++)
        {
            printf("locked[%i][%i] = %i   ", v, vv, locked[v][vv]);
        }
        printf("\n");
    }
    */

    // #6
    print_winner();








    /*
    //you can use this template to check/print pref - 2D array
    for (int z=0; z<candidate_count; z++)
    {
        for (int zz=0; zz<candidate_count; zz++)
        {
            printf("pref [%i][%i] = %i ", z, zz, preferences[z][zz]);
        }
        printf("\n");
    }

    //template to check winner/looser pairs results
    printf("pairs = %i\n", pair_count);
    for (int y = 0; y < pair_count; y++)
    {
        printf("w%i = cand[%i]\n",y, pairs[y].winner);
        printf("l%i = cand[%i]\n",y, pairs[y].loser);
        printf("\n");
    }
    */

    /*




    //#4
    sort_pairs();

    //#5
    for (int ii = 0; ii < candidate_count; ii++)
    {
        for (int jj = 0; jj < candidate_count; jj++)
        {
            locked[ii][ii] = false;
        }
    }
    lock_pairs();

    */
}

// =====================FUNCTIONS==============================================
// #1 Vote - updates ranks[i] array and returns true if updated
bool vote(int rank, string name, int ranks[])
{
    // iterate over candidates[candidate_count] array and check for name. if found put that name's index to ranks[rank] = index
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}


// #2 Update preference 2d array based on ranks array
void record_preferences(int ranks[])
{
    //iterate over ranks[i] array
    for (int i = 0; i < candidate_count - 1; i++)
    {
        // for pref[i][j]: i preference over j, j is starting at i+1 and iterated till end
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}


// #3 - update pairs 2D array using preferences matrix
void add_pairs(void)
{
    int index = 0;
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[index].winner = i;
                pairs[index].loser = j;
                pair_count++;
                index ++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[index].winner = j;
                pairs[index].loser = i;
                pair_count++;
                index ++;
            }
            else
            {
                continue;
            }
        }
    }
}


// #3.1 - victory strength value
void victory_strength(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        win_strength[i] = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
    }
}


// #4-1
void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}


// #4 sorts pairs
void sort_pairs(void)
{
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            if (win_strength[j] < win_strength[j + 1])
            {
                swap(&win_strength[j], &win_strength[j + 1]);
                swap(&pairs[j].winner, &pairs[j + 1].winner);
                swap(&pairs[j].loser, &pairs[j + 1].loser);
            }

        }
    }
}


// #5 update locked 2D array
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (locked[pairs[i].loser][pairs[i].winner] == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
        else
        {
            continue;
        }
    }
}


// #6 - PRINT WINNER
void print_winner(void)
{
    int winner;

    for (int i = 0; i < candidate_count; i++)
    {
        winner = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == false)
            {
                winner++;
            }
        }

        // Prints all the names that are the source of the graph
        if (winner == candidate_count)
        {
            printf("%s\n", candidates[i]);
        }
    }
}
