#include <stdlib.h>
#include <stdio.h>

// Create a grid to track vistited nodes
int **create_visited_grid(int height, int width) {
    int **grid = (int**)malloc(height*sizeof(int*));
    for (int i = 0; i < height; i++) {
        grid[i] = (int*)malloc(width*sizeof(int));
        for (int j = 0; j < width; j++) {
            grid[i][j] = 0;
        }
    }
    return grid;
}

// Free the grid
void free_grid(int **grid, int height) {
    for (int i = 0; i < height; i++) {
        free(grid[i]);
    }
    free(grid);
}

// Check if the cell is valid 
int is_valid_cell(int row, int col, int height, int width) {
    if (row < 0 || col < 0 || row >= height || col >= width) {
        return 0;
    }
    return 1;
}


// Executes DFS on a bmp board and tracks any connections through modifying component_grid 
void dfs(char **bmp_array, int **visited, int **component_grid, int row, int col, int height, int width) {

    // If node has been visited or out of bounds
    if (is_valid_cell(row,col,height,width) == 0 || visited[row][col] == 1) {
        return;
    }
    // Track visited node
    visited[row][col] = 1;
    
    // If the search is currently on a component
    if (component_grid[row][col] == 2) 
    {
        // Change to 3 to track connection 
       component_grid[row][col] = 3; 
       return; 
    }

    // Recursive part of DFS
    if (is_valid_cell(row+1,col,height, width) && bmp_array[row+1][col] == bmp_array[row][col]) {
        dfs(bmp_array, visited, component_grid, row+1, col, height, width);
    }
    if (is_valid_cell(row-1,col,height, width) && bmp_array[row-1][col] == bmp_array[row][col]) {
        dfs(bmp_array, visited, component_grid, row-1, col, height, width);
    }
    if (is_valid_cell(row,col-1,height, width) && bmp_array[row][col-1] == bmp_array[row][col]) {
        dfs(bmp_array, visited, component_grid, row, col-1, height, width);
    }
    if (is_valid_cell(row,col+1,height, width) && bmp_array[row][col+1] == bmp_array[row][col]) {
        dfs(bmp_array, visited, component_grid, row, col+1, height, width);
    }
}

