#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // condition to check if more or less than 2 arguments in command line
    if (argc != 2)
    {
        printf("Enter only one argument\n");
        return 1;
    }

    int arg_len = strlen(argv[1]); //to verify length of argument for further check if any non digit is present
    int sum = 0;  // this var will be less than arg_len if any char is non aphabetic

    //iterate through each char of argument to verify if all elements are characters
    for (int i = 0; i < arg_len; i++)
    {
        if (isdigit(argv[1][i]) != 0)
        {
            sum++;
        }
        else
        {
            printf("%c is not number. Enter proper text: \n", argv[1][i]);
            return 1;
        }
    }

    // condition to check if any nondigit was in argument
    if (sum == arg_len)
    {
        printf("OK, all symbols are proper\n");
    }
    else
    {
        printf("Unproper symbol\n");
    }
    int keys = atoi(argv[1]) % 26;

    // prompt user to enter text
    string text = get_string("Enter you text to crypt: \n");
    int num_text = atoi(text);
    printf("ciphertext: ");

    // loop to print cypher text
    for (int j = 0; j < strlen(text); j++)
    {
        //condition for upper case letters
        if (isalpha(text[j]) != 0 && isupper(text[j]))
        {
            if ((text[j] + keys) >= 90)
            {
                printf("%c", text[j] + keys - 26);
            }
            else
            {
                printf("%c", text[j] + keys);
            }
        }
        //condition for upper case letters
        else if (isalpha(text[j]) != 0 && islower(text[j]))
        {
            if ((text[j] + keys) >= 122)
            {
                printf("%c", text[j] + keys - 26);
            }
            else
            {
                printf("%c", text[j] + keys);
            }
        }
        else
        {
            printf("%c", text[j]);
        }
    }
    //return success
    printf("\n");
    return 0;
}
