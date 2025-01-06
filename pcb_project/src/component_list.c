#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

#include "bitmap.h"
#include "component_display.h"
#include "sorting.h" 
#include "free_functions.h"

#define THRESHOLD_VALUE 127
#define COLOURS 3
#define TEMPLATE_SIZE 128
#define TEMPLATE_XY 32
#define TOTAL_TEMPLATE_BITS 1024
#define ROW_SIZE 7
#define BITS_IN_BYTE 8
#define BYTES_PER_ROW 4
#define WHITE 255
#define BLACK 0
#define LOCATIONS 2
#define BUFFER 256

// Function to open the bmp file

FILE *open_bmp_file(char *filename){
    FILE *fp_bmp;
    fp_bmp = fopen(filename, "rb");
    return fp_bmp;
}

// This function will find a pixels average RGB value and return whether its threhold is a 0 or 1
int avg_rgb_value(Bmp image, int col, int row){
    float average_rgb;
    average_rgb = (image.pixels[col][row][0] + image.pixels[col][row][1] + image.pixels[col][row][2])/3;
    // If the pixel is foregound
    if (average_rgb > THRESHOLD_VALUE)
    {
        return 1;
    }
    // If the pixel is background
    else
    {
        return 0;
    }
}

/* 
Write a function which stores the thresholded image in a new struct
*/
Bmp store_threshold(Bmp image){
    //Malloc the Struct and assign it values
    Bmp threshold_bmp;
    threshold_bmp.height = image.height;
    threshold_bmp.width = image.width;
    threshold_bmp.header = image.header;
    threshold_bmp.pixels = (unsigned char***)malloc(threshold_bmp.height*sizeof(unsigned char**));
    for (int i=0; i<(int)threshold_bmp.height; i++)
    {
        threshold_bmp.pixels[i] = (unsigned char**)malloc(threshold_bmp.width*sizeof(unsigned char*));
        for (int j=0; j<(int)threshold_bmp.width; j++)
        {
            threshold_bmp.pixels[i][j] = (unsigned char*)malloc(sizeof(unsigned char));
        }
    }
    return threshold_bmp;
}

/*
This funciton will threshold the bmp image, leaving all the pixels the be 0's or 1's
If the average RGB value of the pixel is great than 127, then it will be labelled as 1
*/
Bmp threshold_image(Bmp image){
    // Get the array to store the thresholded image
    Bmp threshold_bmp = store_threshold(image);
    // THis nested for loop changes every pixel to a 1 or a 0
    // Loop through each Column
    for (int i=0; i < (int)image.height; i++)
    {
        //Loop through each row
        for (int j=0; j < (int)image.width; j++)
        {
            // Find the pixles average rgb value
            int bin_val = avg_rgb_value(image, i, j);
            // Assign binary values to 2d array
            if (bin_val == 1)
            {
                threshold_bmp.pixels[i][j][0] = WHITE; 
                threshold_bmp.pixels[i][j][1] = WHITE; 
                threshold_bmp.pixels[i][j][2] = WHITE; 
            }
            else
            {
                threshold_bmp.pixels[i][j][0] = BLACK; 
                threshold_bmp.pixels[i][j][1] = BLACK; 
                threshold_bmp.pixels[i][j][2] = BLACK; 
            }        
        }
    }
    return threshold_bmp; 
}

/* 
Store thresholded struct data in a 2d bianry array
0 represents balck, 1 represents white
*/
char **struct_to_bin_array(Bmp thresholded_bmp, Bmp image){
    // Malloc space for the 2d Array
    char **thresh_bin_array = (char**)malloc(thresholded_bmp.height* sizeof(char*));
    for (int k = 0; k < (int)thresholded_bmp.height; k++) 
    {
        thresh_bin_array[k] = (char*)malloc(thresholded_bmp.width * sizeof(char));
    }
    // Turn pixels into binary representations
    for (int i=0; i < (int)thresholded_bmp.height; i++)
    {
        //Loop through each row
        for (int j=0; j < (int)thresholded_bmp.width; j++)
        {
            int bin_val = avg_rgb_value(image, i, j);
            if (bin_val ==1)
            {
                thresh_bin_array[i][j] = bin_val;
            }
            else
            {
                thresh_bin_array[i][j] = bin_val;
            }
        }
    }
    return thresh_bin_array;
}

/*
This function will slide the template over the 2d array of bmp data to see if there is a match
*/
int test_match(char **bmp, char **temp, int y, int x){
    for (int i=0; i<TEMPLATE_XY; i++)
    {
        for (int j=0; j< TEMPLATE_XY; j++)
        {
            // printf("%b -- %b\n",temp[i][j], bmp[y+i][x+j]);
            if (temp[i][j] != bmp[y+i][x+j])
            {
                return 0;
            }
        }
    }
    return 1; 
}

/*
Function contains nested loops which search over the alrger image to find if there is a template match
*/
int **find_template_in_bmp(Bmp thresh_bmp, char **bmp, char **temp, int **match_points){
    // initialise x,y values to be -1, If this gets returned can use this to count the number pf components
    int x=0;
    for (int i=0; i<=(int)thresh_bmp.height - TEMPLATE_XY; i++)
    {
        for (int j=0; j<=(int)thresh_bmp.width - TEMPLATE_XY; j++)
        {
            // If there is a match
            if (test_match(bmp, temp, i, j) ==1)
            {
                // Create spots for locations to be stored 
                match_points[x] = malloc(sizeof(int*));
                match_points[x][0] = i;
                match_points[x][1] = j; 
                x++;
            
            }
        }
    }
    return match_points; 
}

// Find the amount of repeats of a particular template
int find_repeats(Bmp thresh_bmp, char **bmp, char **temp, int repeats){
    // initialise x,y values to be -1, If this gets returned can use this to count the number pf components
    for (int i=0; i<=(int)thresh_bmp.height - TEMPLATE_XY; i++)
    {
        for (int j=0; j<=(int)thresh_bmp.width - TEMPLATE_XY; j++)
        {
            // If there is a match
            if (test_match(bmp, temp, i, j) ==1)
            {
                repeats++;
            }
        }
    }
    return repeats; 
}


/* 
This function sorts the 2d array of the template into a 32x32 array of bits
*/
char **array_sort(char **template_search_array){
    // Allocate memory for new array 
    char **new_temp_array = malloc(sizeof(char *)*TEMPLATE_XY);
    // Iterate through height
    for (int i = 0; i < TEMPLATE_XY ; i++) 
    {
        new_temp_array[i] = malloc(sizeof(char)*TEMPLATE_XY);
        for (int j = 0; j < BYTES_PER_ROW ; j++) 
        {
            for (int k = 0; k < BITS_IN_BYTE; k++) 
            {
                new_temp_array[i][(j * BITS_IN_BYTE) + k] = (template_search_array[i][j] >> (ROW_SIZE - k)) & 1;
            }
        }
    } 
    return new_temp_array; 
}

/*
This function will take in the amount of components that are present on the template
and will order them using qsort
*/

void print_order(int **stored_location, int num_match){   
    // Print the answer
    printf("Found %d components:\n", num_match);
    // Order the locations with respect to rows
    qsort(stored_location, num_match, sizeof(int*), compare);
    for (int i=0; i<num_match; i++)
    {
        printf("type: %d, row: %d, column: %d\n", stored_location[i][2], stored_location[i][0], stored_location[i][1]);
    }
}

/*
Wraps up print process in a function
*/
void print_wrap(int num_match, int **store_location){

    // If there are componenents found, call print fucntion
    if (num_match != 0)
    {
        print_order(store_location, num_match);
    }
    // If there are no components found 
    else
    {
        printf("Found 0 components:\n");
    }
}



// THis funciton will run the entire porogram for mode List 
int bmp_main_func(char *filename, char *temp_filename){
    // Open the file for reading
    FILE *fp_bmp = open_bmp_file(filename);
    // Read in the data from file and put in struct
    Bmp image = read_bmp(filename);
    // Create new Bmp struct which holds the thresholded image data
    Bmp thresholded_bmp = threshold_image(image);
    // Put the thresholded BMP into a 2d array 
    char **bmp_array = struct_to_bin_array(thresholded_bmp, image);
    // Open the template file
    FILE *fp_bin = open_template_file(temp_filename);
    if (fp_bin == NULL)
    {
        return -1;
    }

    // Num of templates in template file
    int number_of_templates = number_of_components(fp_bin);
    int *store_location[BUFFER];
    // Num match is a counter which counts the number of matches we get
    int num_match = 0;
    // Loop through each template 
    for (int i=0; i<number_of_templates; i++)
    {
        FILE *fp_component = template_search(fp_bin, i);
        // Create the 2d array to store the 32x32 template 
        char **temp_array = binary_two_d_store(fp_component);
        // Sort the template from the bottom left as previous func creates it upside down
        char **new_temp_array = array_sort(temp_array);
        //repeats - this represents the amount of times we find a particular comoponent on the pcb board
        int repeats = 0;
        repeats = find_repeats(thresholded_bmp, bmp_array, new_temp_array, repeats);
        //Match points - stores the x and y locations of any template matches on the pcb board 
        int **match_points = malloc((repeats)*sizeof(int*));
        // Loop through location grab process, for the amount of time a component is repeated
        for (int x=0; x<repeats; x++)
        {
            // Allocate 2 integer spots for (row,col)
            match_points[x] = malloc(LOCATIONS*sizeof(int*));
            // Set initial values to -1, if these change, we know there is a match 
            match_points[x][0] = -1;
            match_points[x][1] = -1;
            // Call for function to find location of a match and store it in a 2d array
            int **location = find_template_in_bmp(thresholded_bmp, bmp_array, new_temp_array, match_points);
            // store location in array
            if (location[x][0] != -1)
            {
                //store the component type in qith it's corresponding location
                location[x] = realloc(location[x], sizeof(int));
                location[x][2] = i;
                // store the location in a larger array which will hold all the locations of all matches
                store_location[num_match] = malloc(sizeof(int*));
                store_location[num_match] = location[x];
                num_match++;
            }
        }
        // Free the heap
        free_2d_array_char(temp_array, TEMPLATE_XY);
        free_2d_array_char(new_temp_array, TEMPLATE_XY);
    }  
    // If there are componenents found, call print fucntion
    print_wrap(num_match, store_location);
    // Free the heap
    free_2d_array_loc(store_location, num_match);
    free_2d_array_char(bmp_array, thresholded_bmp.height);  
    free_bmp(thresholded_bmp);
    fclose(fp_bmp);
    return 1;
}
