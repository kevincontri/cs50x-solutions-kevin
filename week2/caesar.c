#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool only_digits(string x);
char rotate(char p, int k);

int main(int argc, string argv[])
{
    string plaintext;
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1; // Exit the program when argc doesn't correspond
    }

    if (only_digits(argv[1]) == true)
    {
        plaintext = get_string("plaintext:  ");

        int key = atoi(argv[1]);
        int length = strlen(plaintext);

        printf("ciphertext: ");
        for (int i = 0; i < length; i++)
        {
            int cipher = rotate(plaintext[i], key);
            printf("%c", cipher);
        }
        printf("\n");
        return 0;
    }
    else if (only_digits(argv[1]) == false)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}

char rotate(char p, int k)
{
    int ciphered_letter;

    if (isupper(p))
    {
        ciphered_letter = (p - 'A' + k) % 26 + 'A';
        return ciphered_letter;
    }
    else if (islower(p))
    {
        ciphered_letter = (p - 'a' + k) % 26 + 'a';
        return ciphered_letter;
    }
    else
    {
        return p;
    }
}

bool only_digits(string x) // Function to iterate through each letter on argv[1] and check if it's not a digit, returning a boolean expression
{
    int length = strlen(x);
    for (int i = 0; i < length; i++)
    {
        if (!isdigit(x[i]))
        {
            return false; // Here we only return true or false, not an exit value, for it's output is of type boolean
        }
    }
    return true;
}
