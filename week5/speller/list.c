#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

#define LIST_SIZE 3

typedef struct node
{
    string phrase;
    struct node* next;
}node;

void visualizer (node* list);
bool unload(node* list);

int main()
{
    node* list = NULL;

    for(int i = 0; i < LIST_SIZE; i++)
    {
        string phrase = get_string("New phrase: ");

        node *n = malloc(sizeof(node));
        if(n == NULL) {fprintf(stderr, "Error\n"); return 1;}

        n->phrase = phrase;
        n->next = list;
        list = n;

        visualizer(list);
    }

    if(!unload(list))
    {
        printf("Error freeing the list\n");
        return 1;
    }

    printf("Freed the list.\n");
    return 0;
}

bool unload(node* list)
{
    node* ptr = list;

    while(ptr != NULL)
    {
        ptr = list->next;
        free(list);
        list = ptr;
    }

    return true;
}



void visualizer (node* list)
{
    printf("\n+-- List Visualizer --+\n\n");
    while(list != NULL)
    {
        printf("Location: %p \n Phrase: %s \n Next: %p \n", list, list->phrase, list->next);
        list = list->next;
    }
    printf("+-----------------+\n\n");
}
