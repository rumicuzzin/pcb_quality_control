#include <stdlib.h>
#include <stdio.h>

void free_2d_array_int(int **array, int rows) {
    // First free each sub-array
    for(int i = 0; i < rows; i++) 
    {
        free(array[i]);
    }
    // Then free the array of pointers
    free(array);
}

void free_2d_array_loc(int **array, int rows) {
    // First free each sub-array
    for(int i = 0; i < rows; i++) 
    {
        free(array[i]);
    }
}


void free_2d_array_char(char **array, int rows) {
    // First free each sub-array
    for(int i = 0; i < rows; i++) 
    {
        free(array[i]);
    }
    // Then free the array of pointers
    free(array);
}


void free_temp_array(char **temp_array, char **new_temp_array, int z){
    // Free Malloc'd data
    for (int x = 0; x < z; x++) 
    {
        free(temp_array[x]);
        free(new_temp_array[x]);
    }
    free(temp_array);
    free(new_temp_array);

}
