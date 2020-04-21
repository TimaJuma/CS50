include <stdio.h>
#include <stdbool.h>
#include <math.h>


//declare functions to be used in main
bool validity(long long card);
void card_type(long long card, int length, int digit_two, int digit_one);

int main(void)
{
    long long card_num;
    do //to check for negative input
    {
        printf("Enter credit card number:\n");
        scanf("%lli", &card_num);
    }
    while (card_num < 0);
    //temporary results
    int l_count = 0;
    long long card_copy = card_num;
    while (card_copy > 1) // identify length of card
    {
        card_copy = card_copy / 10;
        l_count++;
    }
    //identify first two digits
    long long digit_12 = card_num / pow(10, l_count - 2);
    long long digit_1 = card_num / pow(10, l_count - 1);

    //tune variables to functions
    bool valid = validity(card_num);
    if (valid == 0 )
    {
        card_type(card_num, l_count, digit_12, digit_1);
    }
    else
    {
        printf("INVALID\n");
    }
    
}

//function#1 - to check validity of card
bool validity(long long credit_card)
{
    int position = 0, sum_pos_odd = 0, sum_pos_even = 0, total_sum = 0;
    int digit_even, digit_odd;

    // identify two digits at each loop
    while (credit_card > 0)
    {   
        digit_even = credit_card % 10;
        digit_odd = (credit_card / 10) % 10;
        
        sum_pos_even += digit_even;
        digit_odd = 2 * digit_odd;
        //this representation is needed when multiplication exceeds 10
        sum_pos_odd += (digit_odd / 10) + (digit_odd % 10);

        credit_card /= 100;
        position ++;
        total_sum = sum_pos_even + sum_pos_odd;
    }

    if (total_sum % 10 == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


//function#2 - identify type of card
void card_type(long long card, int length, int digit_two, int digit_one)
{
    // condition for VISA
    if (digit_one == 4 && length >= 13 && length <= 16)
    {
        printf("VISA\n");
    }
    //condition for MASTERCARD
    else if (digit_two >= 51 && digit_two <= 55 && length == 16)
    {
        printf("MASTERCARD\n");
    }
    //condition for AMEX
    else if ((digit_two == 34 || digit_two == 37) && length == 15)
    {
        printf("AMEX\n");
    }
    else
    {
        printf("INVALID\n");
    }

    return;
}
