#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int h;
    // prompt user to enter proper height value 
    do
    {
        h = get_int("Enter the height\n");
    }
    while (h < 1 || h > 8);
    int i, j; 
    for (i = 0; i <= h - 1; i++)
    {
        for (j = 0; j < h - i - 1; j++) //spaces
        {
            printf(" ");
        }
        
        for (j = 0; j <= i; j++) //hash before space
        {
            printf("#");
        }  
        printf("  "); //space
        for (j = 0; j <= i; j++) // hash after space
        {
            printf("#");
        }
        printf("\n");  
         
    }

}
