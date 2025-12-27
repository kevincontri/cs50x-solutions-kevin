#include <cs50.h>
#include <stdio.h>

int main()
{
    int height;
    do
    {
        height = get_int("Height ");
    }
    while (height < 1 || height > 8);

    for(int i = height; i > 0; i--)
    {
        for(int j = 0; j < i - 1; j++)
        {
            printf(" ");
        }
        for(int k = 0; k < height - i + 1; k++)
        {
            printf("#");
        }
        printf("\n");
    }
}
