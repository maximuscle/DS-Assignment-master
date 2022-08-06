#include <stdio.h>
#include <stdlib.h>
#include "PES1UG20CS111_H.h"

void getcord(coord *c, FILE *fp)
{
    fscanf(fp, "%d %d\n", &c->x, &c->y);
}

void insert_dnode(r_node *row, d_node *data)
{
    d_node *pres = row->data_node; // pres will point to the first data node

    if (pres == NULL) // first data node of the row to be inserted
        row->data_node = data;
    else
    {
        while (pres->right != NULL) // else traverse till the last data node and insert at end
            pres = pres->right;
        
        pres->right = data;
    }
}

// Searches for data node, returns pointer to the node if exists else returns NULL
d_node *search(r_node *head, int x, int y)
{
    d_node *data;
    // Traverse to the correct row
    while (head != NULL && head->index != x)
    {
        head = head->next;
    }

    if (head != NULL)
    {
        data = head->data_node;
        // Traverse through the columns
        while (data != NULL && data->cord.y != y)
        {
            data = data->right;
        }
        return data;
    }
    else
        return NULL;
}

r_node *read_map(FILE *fp)
{
    char ch;
    int flag = 1;
    int row_no = 0;
    int index = 0;
    r_node *temp_row, *first, *prev_row = NULL;
    d_node *temp_data;

    while ((ch = fgetc(fp)) != EOF)
    {
        if (flag) // create row_node at beginning
        {   
            temp_row = (r_node *) malloc(sizeof(r_node));
            temp_row->index = row_no;
            temp_row->data_node = NULL;
            temp_row->next = NULL;

            if (prev_row == NULL) // if it is the first row node to be inserted
                first = temp_row;
            else
                prev_row->next = temp_row; // else link the prev row node to the newly created row node

            flag = 0;
        }
        
        switch (ch)
        {
            case '\n': // at the end of each line
                prev_row = temp_row;
                row_no++;
                flag = 1;
                index = 0;
                break;
                
            case '0':
                temp_data = (d_node *) malloc(sizeof(d_node));
                temp_data->right = temp_data->down = NULL;
                temp_data->visited = 0;
                temp_data->cord.x = row_no;
                temp_data->cord.y = index;

                insert_dnode(temp_row, temp_data);
                index++;
                break;
            
            case '1':
                index++;
                break;
            
            default: // empty space, ignore
                break;
        }
    }

	find_down(first);
    return first;
}

void display(r_node *head)
{
    d_node *data;

    printf("\n");
    while (head != NULL)
    {
        data = head->data_node;
        printf("%d: ", head->index);
        while (data != NULL)
        {
			if (data->right == NULL) // (%d, %d, %d) 3rd %d for checking visited flag
            	printf("(%d, %d)", data->cord.x, data->cord.y);
			else
            	printf("(%d, %d) -> ", data->cord.x, data->cord.y);
            data = data->right;
        }
        printf("\n");
    	head = head->next;
    }
}

void link_down(r_node *row, d_node *data) // linking down part of the data node if exists
{
    d_node *nr_data; // next row's data node
    while (row->next != NULL)
    {
    	row = row->next;
		nr_data = row->data_node;

		while (nr_data != NULL && (nr_data->cord.y != data->cord.y))
			nr_data = nr_data->right;

		if (nr_data != NULL) // stopping for the condition where it matches
		{
			// nr_data is the next down value to data
			data->down = nr_data;
			break;
		}
    }
}

void find_down(r_node *head) // traversing each data node and finding down part of the data node if exists
{
    d_node *data;
    
    while (head->next != NULL) // traverse until the last row
    {
        data = head->data_node;
        while (data != NULL)
        {
            link_down(head, data);
            data = data->right;
        }
        head = head->next;
    }
}

int move_right(d_node *data)
{
    if (data->right != NULL)
    {
        if (data->right->cord.y == data->cord.y + 1)
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

int move_down(d_node *data)
{
    if (data->down != NULL)
    {
        if (data->down->cord.x == data->cord.x + 1)
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

void display_stack(stack_e *stack_head)
{
    while (stack_head != NULL)
    {
        printf("\n(%d, %d, %d)", stack_head->data->cord.x, stack_head->data->cord.y, stack_head->data->visited);
        stack_head = stack_head->next;
    }
}

void push(stack_e **stack_head, d_node *data, stack_e **top)
{
    stack_e *temp = (stack_e *) malloc(sizeof(stack_e));
    temp->next = temp->prev = NULL;
    temp->data = data;

    if (*stack_head == NULL)
    {
        *stack_head = temp;
        *top = *stack_head;
    }
    else
    {
        temp->prev = *top;
        (*top)->next = temp;
        *top = temp;
    }
}

void pop(stack_e **stack_head, stack_e **top)
{
    stack_e *to_pop = *top;
    if ((*top)->prev != NULL) // if the block to be freed is not the first element of stack
    {
        *top = (*top)->prev;
    }
    else
    {
        *stack_head = *top = NULL;
    }
    free(to_pop);
}

void find_path(r_node *head, coord *start, coord *end, stack_e **stack_head)
{
    d_node *bot_ptr = search(head, start->x, start->y);
    d_node *end_ptr = search(head, end->x, end->y);
    stack_e *top = NULL;

    if (end_ptr == NULL)
    {
        printf("\nEnding Coordinates Not Found.");
    }
    else if (bot_ptr != NULL)
    {
        // Pushing starting node to stack
        bot_ptr->visited = 1;
        push(stack_head, bot_ptr, &top);

        while (bot_ptr != NULL && (bot_ptr->cord.x != end->x || bot_ptr->cord.y != end->y))
        {
            // condition where the y coordinate would match but x is still unmatched
            if (bot_ptr->cord.y <= end->y && bot_ptr->cord.x != end->x)
            {    
                if (move_right(bot_ptr) && (bot_ptr->right->visited != 1))
                {
                    bot_ptr = bot_ptr->right;
                    bot_ptr->visited = 1;
                    push(stack_head, bot_ptr, &top);
                }
                else if (move_down(bot_ptr) && (bot_ptr->down->visited != 1))
                {
                    bot_ptr = bot_ptr->down;
                    bot_ptr->visited = 1;
                    push(stack_head, bot_ptr, &top);
                }
                else
                {
                    pop(stack_head, &top);
                    if (top != NULL)
                        bot_ptr = top->data;
                    else
                    {
                        bot_ptr = NULL;
                    }
                }
            }
            else
            {
                if (move_down(bot_ptr) && (bot_ptr->down->visited != 1))
                {
                    bot_ptr = bot_ptr->down;
                    bot_ptr->visited = 1;
                    push(stack_head, bot_ptr, &top);
                }
                else if (move_right(bot_ptr) && (bot_ptr->right->visited != 1))
                {
                    bot_ptr = bot_ptr->right;
                    bot_ptr->visited = 1;
                    push(stack_head, bot_ptr, &top);
                }
                else
                {
                    pop(stack_head, &top);
                    if (top != NULL)
                        bot_ptr = top->data;
                    else
                    {
                        bot_ptr = NULL;
                    }
                }
            }
        }
    }
    else
    {
        printf("\nStarting Coordinates Not Found.");
    }
}

void store_path(stack_e *stack_head)
{
    FILE *fpout = fopen("output.txt", "w+");

    if (stack_head != NULL)
    {
        printf("\nPATH FOUND.\n");
        while (stack_head != NULL)
        {
            fprintf(fpout, "(%d, %d)\n", stack_head->data->cord.x, stack_head->data->cord.y);
            stack_head = stack_head->next;
        }
    }
    else
    {
        printf("\nNO PATH FOUND.\n");
        fprintf(fpout, "%d", -1);
    }

    if (fpout != NULL)
        printf("\nOutput file generated.\n");
    else
        printf("\nFailed to generate output file\n");
}
