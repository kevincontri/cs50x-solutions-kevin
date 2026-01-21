#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int hash(string phrase);

int main()
{

    for(int i = 0; i < 3; i++)
    {
        string phrase = get_string("New phrase to be hashed: ");

        printf("%s is hashed in %i\n", phrase, hash(phrase));
    }
}

int hash(string phrase)
{
    return toupper(phrase[0]) - 'A';
}
