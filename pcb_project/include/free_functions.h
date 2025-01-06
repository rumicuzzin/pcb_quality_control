/*
Functions which free arrays that have been allocated on the heap
*/ 

#ifndef FREE_FUNCTIONS_H
#define FREE_FUNCTIONS_H

// This function is called when we need to free data of a 2d array
void free_2d_array_int(int **array, int rows);
// Does same except for char array 
void free_2d_array_char(char **array, int rows);
// Frees the location array, 
void free_2d_array_loc(int **array, int rows);
// Frees 2 two dimensional arrays at once
void free_temp_array(char **temp_array, char **new_temp_array, int z);

#endif
