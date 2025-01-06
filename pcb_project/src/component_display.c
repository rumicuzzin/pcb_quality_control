#include <stdlib.h>
#include <stdio.h>

#include "bitmap.h"
#include "component_list.h"
#include "free_functions.h"

#define TEMPLATE_SIZE 128
#define TEMPLATE_XY 32
#define TOTAL_TEMPLATE_BITS 1024
#define ROW_SIZE 7
#define BYTES_PER_ROW 4

// Open the tmeplate File

FILE *open_template_file(char *filename){
    // Set the file pointer
    FILE *fp_bin;
    // Open the binary file in read mode
    fp_bin = fopen(filename, "rb");
    // If the filename is not found 
    return fp_bin;
}

/* 
Read in the first byte in the template to find how many components
*/

int number_of_components(FILE *fp){
    // n equals the number of components
    unsigned char n_temp; 
    size_t succesfull_read = fread(&n_temp,1,1,fp);

    // If the byte is succesfuly read
    if (succesfull_read)
    {
        int n = n_temp;
        return n; 
    }
    // If the first byte does not exist in the file
    else
    {
        printf("Corrupt File");
        exit(0);
    }
}

/*
Find the particular component that the user wants to print 
*/
FILE *template_search(FILE *fp, int index){
    // Find out any offset
    long offset = 1 + index * TEMPLATE_SIZE; 
    // Set the file pointer to the position of the nessecary data
    fseek(fp, offset, SEEK_SET);
    return fp;   
}


// This one is for storing the array 
FILE *template_search_array(FILE *fp, int index){
    // Find out any offset
    long offset = 1 + index * BYTES_PER_ROW; 
    // Set the file pointer to the position of the nessecary data
    fseek(fp, offset, SEEK_SET);
    return fp;   
}


/*
Print the corresponding bytes of binary data from the top left.
*/

// Helper function to print bits of a byte
void print_bit(unsigned char byte) {
    for (int i=ROW_SIZE; i>=0; i--)
    {
        if ((byte >> i) & 1)
        {
            printf("1");
        }
        else
        {
            printf(" ");
        }
    }
}

// Prints the binary digits from the array 
void binary_print(char **component_array){
    printf("Template data:\n");
    for (int i=TEMPLATE_XY-1; i>=0; i--)
    {
        for (int j=0; j<BYTES_PER_ROW; j++)
        {
            print_bit(component_array[i][j]);
        }
        printf("\n");
    }
}


/*
Reads in the binary data from the template file and stores it in an ordered 2d array
*/

char **binary_two_d_store(FILE *fp){
    // Create an 1d array which acts as a buffer - reading in all the binary data 
    char *bin_array = (char*)malloc(TEMPLATE_SIZE*sizeof(char));
    size_t bytes_read = fread(bin_array, 1, TEMPLATE_SIZE, fp);
    //  If there is the sufficient amount of bytes for a template
    if (bytes_read != TEMPLATE_SIZE)
    {
        printf("Invalid!");
    }

    // Create a 2D array which represents the template image in x-y components
    char **two_d_array = (char**)malloc(TEMPLATE_XY * sizeof(char*));
    for (int k = 0; k < TEMPLATE_XY; k++) 
    {
        two_d_array[k] = (char*)malloc(BYTES_PER_ROW * sizeof(char));
    }
    // Loop through the template and store it in a 2d array
    for (int j=TEMPLATE_XY-1; j>=0; j--)
    {
        // Loop through the row values in the correct manour so it lines up (0,0) for example
        for (int i=TEMPLATE_SIZE+((j-TEMPLATE_XY)*BYTES_PER_ROW); i<=TEMPLATE_SIZE+((j-TEMPLATE_XY+1)*BYTES_PER_ROW); i++)
        {
            // Store the data in the 2D array from bottom left
            int x = i - TEMPLATE_SIZE -(((j-TEMPLATE_XY))*BYTES_PER_ROW);
            two_d_array[j][x] = bin_array[i];
        }
    }
    return two_d_array;
    // Free Array
    free(bin_array);
}

// Compiles all the functions together to exevcute task 
int main_template_func(char *filename, int index){
    // open the file for reading
    FILE *fp_bin = open_template_file(filename);
    // If the filename doen't exist
    if (fp_bin == NULL)
    {
        return -1;
    }
    // Store the amount of components
    int num = number_of_components(fp_bin);
    // if template index out of range 
    if (index >= num)
    {
        return 0;
    }
    // Get pointer to component
    FILE *component_fp;
    component_fp = template_search(fp_bin, index);
    char **two_d_array = binary_two_d_store(component_fp);
    // Call functiuon to print the template data 
    binary_print(two_d_array);
    // Free the array of template data from the heap
    free_2d_array_char(two_d_array, BYTES_PER_ROW);
    // Close the binary file
    fclose(component_fp);
    return 1;
}





