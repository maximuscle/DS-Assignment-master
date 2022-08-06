#include <stdio.h>
#include <strings.h>

typedef struct coordinates
{
    int x;
    int y;
} coord;

typedef struct data_node
{
    coord cord;
    int visited;
    struct data_node *right;
    struct data_node *down;
} d_node;

typedef struct row_node
{
    int index;
    d_node *data_node;
    struct row_node *next;
} r_node;

typedef struct stack_element
{
    d_node *data;
    struct stack_element *next;
    struct stack_element *prev;
} stack_e;

// Initialization
void getcord(coord *, FILE *);
d_node *search(r_node *, int, int);

// Building Multilinked list
void insert_dnode(r_node *, d_node *);
r_node *read_map(FILE *);
void link_down(r_node *, d_node *);
void find_down(r_node *);

// Display Functions
void display(r_node *);
void display_stack(stack_e *);

// Path finding and movement
int move_right(d_node *);
int move_down(d_node *);
void find_path(r_node *, coord *, coord *, stack_e **);
void store_path(stack_e *);

// Stack operations
void push(stack_e **, d_node *, stack_e **);
void pop(stack_e **, stack_e **);