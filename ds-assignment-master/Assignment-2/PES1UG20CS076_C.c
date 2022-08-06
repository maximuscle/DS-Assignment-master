#include "PES1UG20CS111_H.h"

int main()
{
    int vertices;
    int dfsres = 0;
    int bfsres = 0;
    int startv, endv;
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
    
    vertices = num_v(fp);
    v_node *adj_list = (v_node *) malloc((vertices + 1) * sizeof(v_node));

    read_map(fp, adj_list);
    
    printf("\nDisplaying Adjacency List.\n");
    display_adj_list(adj_list, vertices);

    printf("\n\nFinding path:\n");
    int *dfspath = (int *) calloc(vertices, sizeof(int));
    int *bfspath = (int *) calloc(vertices, sizeof(int));

    // Storing the starting and the ending coordinates
    startv = find_vertex(adj_list, &start, vertices);
    endv = find_vertex(adj_list, &end, vertices);

    find_path(adj_list, startv, endv, vertices, &dfsres, &bfsres, dfspath, bfspath);
    store_path(adj_list, startv, endv, vertices, dfsres, bfsres, dfspath, bfspath);

    free(adj_list);
    return 0;
}
