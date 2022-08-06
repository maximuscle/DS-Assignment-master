#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

typedef struct coordinates
{
    int x;
    int y;
} coord;

typedef struct vertex_node
{
    int vid;
    coord loc;
    struct vertex_node *next;
} v_node;

// reading from file
void getcord(coord *c, FILE *fp);
int num_v(FILE *fp);
void read_map(FILE *fp, v_node *adj_list);
void create_adj_list(v_node *adj_list, int vertices);
void insert_vnode(v_node *adj_list, int src, int dest);

// display functions
void display_adj_list(v_node *adj_list, int vertices);

// finding path
int find_vertex(v_node *adj_list, coord *loc, int vertices);
void find_path(v_node *adj_list, int startv, int endv, int vertices, int *dfsres, int *bfsres, int *dfspath, int *bfspath);
int dfs(v_node *adj_list, int s, int d, int *visited, int length, int *path);
void bfs(v_node *adj_list, int s, int d, int *visited, int vertices, int *prev);
int reconstruct_from_bfs(v_node* adj_list, int startv, int endv, int *bfspath, int *prev);
void store_path(v_node *adj_list, int startv, int endv, int vertices, int dfsres, int bfsres, int *dfspath, int *bfspath);

// queue functions
void qinsert(int *q, int *f, int *r, int k);
int qdelete(int *q, int *f, int *r);
int qisempty(int *f);
