#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main()
{
    string text = get_string("Text: ");
    int length = strlen(text);
    int letters = 0, words = 1, sentences = 0;
    double avrg_letters = 0, avrg_sentences = 0;

    for (int i = 0; i < length; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
        if (isspace(text[i]))
        {
            words++;
        }
        if (text[i] == '.' || text[i] == '!' ||
            text[i] == '?') // Not using ispunct because only these three punctuations are allowed.
        {
            sentences++;
        }
    }

    // Calculate the average
    avrg_letters = (double) letters / words * 100; // Double because it can handle decimal numbers.
    avrg_sentences = (double) sentences / words * 100;

    double index = 0.0588 * avrg_letters - 0.296 * avrg_sentences - 15.8;

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
        printf("Grade %d\n",
               (int) round(
                   index)); // Turned into an int and rounded it to the nearest integer from index.
    }
}
