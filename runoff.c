#include <cs50.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

int candidate_count;  // we will use these integers in multiple iterations in our functions, so they are made global
int voter_count;
candidate candidates[MAX_CANDIDATES]; // datatype of candidates
int preferences[MAX_VOTERS][MAX_CANDIDATES]; //array of voters vote by rank

//functions to be used:
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);




//===============================================================================================
int main(int argc, string argv[])
{
    if (argc < 2) // condition to check no argument
    {
        printf("Please enter candidates name when runnig program in format of ./runoff ...\n");
        return 1;
    }

    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES) // check for votes number of candidates lower than MAX
    {
        printf("Number of candidates shall not exceed %i\n", MAX_CANDIDATES);
        return 2;
    }

    //indicate # of voters
    voter_count = get_int("Number of voters:\n");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters that can vote is %i\n", MAX_VOTERS);
        return 3;
    }

    // create initial matrix of candidates with 0 votes
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    //input from voters by ranks and tune vote function to update preferences 2x2 array
    string names;
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            names = get_string("Rank %i:\n", j + 1);
            vote(i, j, names);
            if (!vote(i, j, names))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }
    }

    //===========tune vars to functions to check the winner=================================

    // this part of code iterates until winner is found or tie

    int loops = 0;
    while (loops < candidate_count)
    {
        tabulate(); // update number of votes in candidate array
        bool winner = print_winner();
        if (winner == true)
        {
            break;
        } // finish program if winner found
        int minimum = find_min();
        bool tie = is_tie(minimum);
        if (tie == true)
        {
            break;
        } // finish program if winner found
        eliminate(minimum);
        loops++;


    }

}
//==============================================================
//function#1 vote
bool vote(int voter, int rank, string name)
{
    // condition when candidate name found - return TRUE
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false; // if no name found return FALSE
}



// function#2 - tabulate
void tabulate(void)
{
    //update votes for non-eliminated candidates
    for (int i = 0; i < voter_count; i++)
    {
        int j = 0;
        while (candidates[preferences[i][j]].eliminated == true)
        {
            j++;
        }
        candidates[preferences[i][j]].votes ++;
    }
}

//function#3 - to find if smb has more than half votes
bool print_winner(void)
{
    int max_votes = voter_count / 2;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > max_votes)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

//function#4 - find minimum votes within candidates with elim = false
int find_min(void)
{
    int min_votes = MAX_VOTERS;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes < min_votes && candidates[i].eliminated == false)
        {
            min_votes = candidates[i].votes;
        }
    }
    return min_votes;
}

//function#5 - find if tie case
bool is_tie(int min)
{
    int num_elim = 0;
    int num_votes = 0;
    for (int i = 0; i < candidate_count; i++)
    {

        if (candidates[i].eliminated == false)
        {
            num_elim++;
        }
        if (candidates[i].votes == min)
        {
            num_votes++;
        }
    }
    if (num_elim == num_votes)
    {
        return true;
    }
    else
    {
        return false;
    }

}

//function#6 eliminate candidate from election with minimum votes
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min && candidates[i].eliminated == false)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}
