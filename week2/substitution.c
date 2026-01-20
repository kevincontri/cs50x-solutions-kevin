#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool check_key(string s);

int main(int argc, string argv[])
{
    string plaintext;

    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (strlen(argv[1]) != 26)
    {
        printf("The key must contain 26 characters!\n");
        return 1;
    }
    else if (check_key(argv[1]) == false)
    {
        return 1;
    }
    else if (check_key(argv[1]) == true)
    {
        plaintext = get_string("plaintext:  ");
    }

    string key = argv[1];
    printf("ciphertext: ");

    for (int i = 0; i < strlen(plaintext); i++)
    {
        if (isupper(plaintext[i]))
        {
            int result = plaintext[i] - 'A'; //The uppercase letter diminished by 65 ('A') will give us the according key[] index;
            printf("%c", toupper(key[result])); //We turn it to uppercase inside the key[] because the function must be case-insensitive.
        }
        else if (islower(plaintext[i]))
        {
            int result = plaintext[i] - 'a'; //Same here
            printf("%c", tolower(key[result]));
        }
        else if (!isalpha(plaintext[i])) //If is not a letter, just print out as is.
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
}

bool check_key(string s)
{
    int length = strlen(s);
    for (int i = 0; i < length; i++)
    {
        if (!isalpha(s[i])) // Check if a character is not a letter, and if it isn't, print an error
                            // message and return false to main();
        {
            printf("The key must contain only letters!\n");
            return false;
        }

        for (int j = i + 1; j < length; j++) // Check if the letters repeat themselves along the
                                             // key;
        {
            if (toupper(s[i]) ==
                toupper(s[j])) // I need to put the letters in uppercase, to check if the letters
                               // repeat themselves case-insensitively. If I didn't put toupper(), 'a'
                               // and 'A' would be considered two different letters.
            {
                printf("Can't have repeated letters!\n");
                return false;
            }
        }
    }
    return true;
}
