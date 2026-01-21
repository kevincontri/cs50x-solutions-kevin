#include <stdio.h>
#include <cs50.h>

int main()
{
    int number_of_people = get_int("Number of people: \n");

    typedef struct
    {
        const char* name;
        const char* number;
        const char* location;
    } person;

    person people[number_of_people];

    for(int i = 0; i < number_of_people; i++)
    {
        string name1 = get_string("Name: ");
        string number1 = get_string("Number: ");
        string location1 = get_string("Location: ");

        printf("\n");

        people[i].name = name1;
        people[i].number = number1;
        people[i].location = location1;
    }

    for(int i = 0; i < number_of_people; i++)
    {
        printf("Name: %s, Number: %s, Location: %s\n", people[i].name, people[i].number, people[i].location);
    }
}
