#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

//anounce functions before main function
int letter_count(string text);
int word_count(string text);
int sentence_count(string text);


//main part
int main(void)
{
    string article = get_string("Enter you text for analysis:\n");
    float num_letters = letter_count(article);
    float num_words = word_count(article);
    float num_sentence = sentence_count(article);

    //calculating index:= 0.0588 * L - 0.296 * S - 15.8
    float index = 0.0588 * ((float) num_letters / (float) num_words * 100) - 0.296 * (100 * (float) num_sentence / (float) num_words) - 15.8;

    //output of the results
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.0f\n", round(index));
    }

}
//END OF MAIN PART

// function#1 for counting LETTERS
int letter_count(string text)
{
    int letters = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (tolower(text[i]) >= 'a' && tolower(text[i]) <= 'z')
        {
            letters++;
        }
    }
    return letters;
}


// function#2 for counting WORDS
int word_count(string text)
{
    int words = 1;
    for (int i = 0; i < strlen(text); i++)
    {
        if (i < strlen(text) - 1 && isspace(text[i]) && isalnum(text[i + 1]))
        {
            words++;
        }
    }
    return words;
}


// function#3 for counting SENTENCES
int sentence_count(string text)
{
    int sentence = 0;
    for (int i = 0; i < strlen(text); i++)
        if (i > 0 && (text[i] == '!' || text[i] == '?' || text[i] == '.') && isalnum(text[i - 1]))
        {
            sentence++;
        }
    return sentence;
}
