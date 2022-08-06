#include <stdio.h>
#include <stdlib.h>
#include "PES1UG20CS111_H.h"

int main()
{
    r_node *head = NULL;
    stack_e *stack_head = NULL;
    FILE *fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("\nInput file does not exist.");
        exit(EXIT_FAILURE);
    }

    printf("\nReading from input file.\n");
    coord start, end;
    getcord(&start, fp);
    getcord(&end, fp);

    printf("\nStart: (%d, %d)\nEnd: (%d, %d)\n", start.x, start.y, end.x, end.y);
    
    printf("\nCreating multilinked list.\n");
    head = read_map(fp);
    fclose(fp);    

    display(head);

    printf("\nFinding Path.\n");
    find_path(head, &start, &end, &stack_head);
    
    // display_stack(stack_head);
    store_path(stack_head);
}