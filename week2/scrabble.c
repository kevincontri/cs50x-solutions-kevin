#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// CS50's "Scrabble" problem set.
int calculate_points(string x, int array[]);

int main()
{
    int const number_of_letters = 26;

    int letter_scores[number_of_letters] = {1, 3, 3, 2,  1, 4, 2, 4, 1, 8, 5, 1, 3,
                                            1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");
    int player1_score = 0;
    int player2_score = 0;

    player1_score = calculate_points(
        word1,
        letter_scores); // It places the user's input and the array as arguments for the function
    player2_score = calculate_points(word2, letter_scores);

    if (player1_score > player2_score)
    {
        printf("Player 1 wins!\n");
    }
    else if (player1_score < player2_score)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int calculate_points(string x, int array[])
{
    int length = strlen(x); // Gets the length of the string provided
    int index = 0;
    int result = 0;
    for (int i = 0; i < length; i++)
    {
        if (isalpha(x[i])) // Checks if every character is a letter with "isalpha(x[i])"
        {
            index += tolower(x[i]) -
                     'a'; // Every letter is turned into lowercase ones and subtracted by 'a',
                          //  which is ASCII code is 97, then getting the index.
            result += array[index]; // Then the index is used as argument for the array and stored
                                    // in result.
            index = 0;              // index is resetted for the next loop iteration
        }
    }
    return result; // returns the stored values in result to complete the computation of the scores.
}
