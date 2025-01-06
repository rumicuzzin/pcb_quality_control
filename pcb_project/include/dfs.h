/*
Functions to execute Depth First search algorithm 
*/

#ifndef DFS_H
#define DFS_H

// Create a grid to track vistited nodes
int **create_visited_grid(int height, int width);

// Free the grid
void free_grid(int **grid, int n);

// Check if the cell is valid 
int is_valid_cell(int row, int col, int height, int width);

// DFS function which is called recursively. Updates any template locations by modyfying values in component grid
void dfs(char **bmp_array, int **visited, int **component_grid, int row, int col, int height, int width);

#endif
