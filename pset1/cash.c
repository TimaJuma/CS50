#include <cs50.h>
#include <math.h>
#include <stdio.h>

void greedy(int change); //declare a function to be used

int main(void)
{
    float cash; // declare a cash var
    do // condition to check for positive input
    {
        cash = get_float("How much change is owed: \n");
    }
    while (cash < 0);
    int change = round(cash * 100);
    greedy(change);
}

void greedy(int change)
{
    int quarters = 0, dimes = 0, nickels = 0, pennies = 0;
    while (change > 24) // for quarters
    {
        change = change - 25;
        quarters++;
    }
    while (change > 9) // for dimes
    {
        change = change - 10;
        dimes++;
    }
    while (change > 4) // for nickels
    {
        change = change - 5;
        nickels++;
    }
    while (change > 0) // for pennies
    {
        change = change - 1;
        pennies++;
    }
    int total_coins = quarters + dimes + nickels + pennies;
    printf("%i\n", total_coins);
    return;
}
