#include <cs50.h>
#include <stdio.h>
#include <string.h>

//create structure for candidate, which includes name and # of votes
typedef struct
{
    string name;
    int votes;
}
candidate;


const int MAX = 9;
int candidate_count;
candidate candidates[MAX]; //new array of candidate type

//introduce functions to be used
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
        //if no candidates entered return
    if (argc < 2)
    {
        printf("Please enter canditates names after './plurality'\n");
        return 1;
    }

    candidate_count = argc - 1;
    if (candidate_count > MAX) // check for votes number of candidates lower than MAX
    {
        printf("Number of votes shall not exceed %i times", MAX);
        return 2;
    }

    //indicate # of votes and promt user n times
    int number = get_int("Enter how many time you wish to vote:\n");

    // assign candidates
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }


    //call vote function n-times to count the votes
    for (int j = 0; j < number; j++)
    {
        string name = get_string("Vote:");
        vote(name);
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }


    //print winner
    print_winner();
}


//function#1: look for candidate and increment the vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes ++;
            return true;
        }
    }
    return false;

}

//function#2: print winner
void print_winner(void)
{
    int maxvote = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > maxvote)
        {
            maxvote = candidates[i].votes;
        }
    }
    for (int j = 0; j < candidate_count; j++)
    {
        if (candidates[j].votes == maxvote)
        {
            printf("%s\n", candidates[j].name);
        }
    }
    return;
}
