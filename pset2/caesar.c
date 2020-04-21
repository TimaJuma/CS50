#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // condition to check if more or less than 2 arguments in command line
    if (argc == 2)
    {
        int arg_len = strlen(argv[1]); //to verify length of argument for further check if any non digit is present
        int sum = 0;  // this var will be less than arg_len if any char is non digit

        //iterate through each char of argument to verify if all are digit ot not
        for (int i = 0; i < arg_len; i++)
        {
            if (isdigit(argv[1][i])!=0)
            {
                sum++;
            }
            else
            {
                printf("%c is not number. Enter proper number: \n", argv[1][i]);
                return 1;
            }

        // condition to check if any nondigit was in argument
        if (sum == arg_len)
        {
            int keys = atoi(argv[1]) % 26;
            printf("OK, all symbols are proper\n");
            string text = get_string("Enter you text to crypt: \n");
            int num_text = atoi(text);
            printf("ciphertext: ");
            for (int j = 0; j < strlen(text); j++)
            {
                if (isalpha(text[j]) !=0 && isupper(text[j]))
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
               else if (isalpha(text[j]) !=0 && islower(text[j]))
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
        }
        else
        {
            printf("Unproper symbol\n");
        }

        }
        printf("\n");
        return 0;
    }
    else
    {
        printf("Enter only one argument\n");
        return 1;
    }

}
