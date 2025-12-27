#include <cs50.h>
#include <stdio.h>

int main() // This code give the user the number of coins they should receive as change, including quarters, dimes, nickles or pennies.
{
    int cents;
    do
    {
        cents = get_int("Change owed: ");
    }
    while (cents < 1); // Prompting the user for a valid number

    int coins = 0;
    const int quarters = 25;
    const int dimes = 10;
    const int nickles = 5;
    const int pennies = 1;

    while (cents >= quarters)
    {
        int result = cents - quarters;
        coins++;
        cents = result;
    }

    while (cents >= dimes)
    {
        int result = cents - dimes;
        coins++;
        cents = result;
    }

    while (cents >= nickles)
    {
        int result = cents - nickles;
        coins++;
        cents = result;
    }

    while (cents >= pennies)
    {
        int result = cents - pennies;
        coins++;
        cents = result;
    }

    if(coins > 15)
    {
        printf("%i coins! That's a lot!\n", coins);
    }
    else
    {
        printf("%i coins\n", coins);
    }
}
