/*
When called mode "t". This file will include all the functions that print a particular component that is stated via index in the command line. 
*/

#ifndef COMPONENT_DISPLAY_H
#define COMPONENT_DISPLAY_H

// Opens the template file
FILE *open_template_file(char *filename);

// Finds the amount of components included in the template file
int number_of_components(FILE *fp);

// Retrurns a pointer to the first bit of data for a particular template, based off the index given in command line
FILE *template_search(FILE *fp, int index);

// Reads in the binary data and stores it in a 2d array on the heap
char **binary_two_d_store(FILE *fp);

// Iterates through every arrray location and prints out data in the required order
void binary_print(char **two);

// Prints out the 8 bits in every byte, used inside binary print function
void print_bit(unsigned char byte);

// Main func for the "t" mode - prints the desired template, returns 0 if unsuccesful
int main_template_func(char *filename, int index);

#endif
