/*
 When called mode "l", the program uses the template library file (argument 2) and the PCB image file (argument 3) to identify any components on the PCB (using template matching) and list their positions and types according to the library.
 */

#ifndef COMPONENT_LIST_H
#define COMPONENT_LIST_H

// Opens the given BMP file for reading
FILE *open_bmp_file(char *filename);

// Thresholds the image, returns a new BMP struct which is black and white
Bmp threshold_image(Bmp image);

// Stores the thresholded image data in a 2d array
char **struct_to_bin_array(Bmp thresholded_bmp, Bmp image);

// Sorts array from 4 bytes in each row to 32 bits in each row - easier in comparison
char **array_sort(char **template_search_array);

// Finds if any components occur more than once on a Bmp image
int find_repeats(Bmp thresh_bmp, char **bmp, char **temp, int repeats);

// Uses Window sliding method to find if templates are in the bmp image, locations are returned
int **find_template_in_bmp(Bmp thresh_bmp, char **bmp, char **temp, int **match_points);

// sorts the locations in row order an prints necessary statements
void print_order(int **stored_location, int num_match);

// Determines if there are any matches or no matches, uses print_order if there are
void print_wrap(int num_match, int **store_location);

// Main function which executes all the functions necessary to complete mode "l"
int bmp_main_func(char *filename, char *temp_filename);

#endif
