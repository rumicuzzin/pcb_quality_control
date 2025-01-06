#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

#include "bitmap.h"
#include "component_display.h"
#include "component_list.h"
#include "sorting.h"
#include "dfs.h"
#include "free_functions.h"

#define TEMPLATE_XY 32
#define LOCATIONS 2
#define BUFFER 256

/*
Converts all the component locations on image array to there corresponing type value
*/
void component_assign(int num_match, int **store_location,int **two_darray){
    for (int i=0; i< num_match; i++)
    {
        for (int x=0; x<TEMPLATE_XY; x++)
        {
            for (int y=0; y<TEMPLATE_XY; y++)
            {
                // Let it equal + 1 so qe can distinguis between the 0's
                two_darray[store_location[i][0]+x][store_location[i][1]+y] = 2;
            }
        }
    }
}

/*
Write a function which 0's out the particluar component we are searching on, so the DFS doesn't return automatically
*/
void zero_out_function(int **component_grid, int **store_location, int q){
    for (int x=0; x<TEMPLATE_XY; x++)
    {
        for (int y=0; y<TEMPLATE_XY; y++)
        {
            
            component_grid[store_location[q][0]+x][store_location[q][1]+y] = 0;
        }
    }

}

/*
Check if there is a 3 on the border of the template
*/
int border_check(int**locations, int k, int j, int x){
    for (int i=0; i<TEMPLATE_XY; i++)
    {
        for (int m=0; m<TEMPLATE_XY; m++)
        {
            if(locations[x][0] + i == j && locations[x][1] + m == k)
            {
                return 1;
            }
        }
    }
    return 0;
}

/*
This function will take in the component grid and be able to determine which positions have 3's at them. The 3's are given in the DFS function to a particular location where a connected component is found
*/
void check_connections(int **component_grid, int **locations, int num_match, int height, int width, int **store_connections, int *successful_connect){

    for (int j=0; j<height; j++)
        {
            for (int k=0; k<width; k++)
            {
                if (component_grid[j][k] == 3)
                {
                    // Check what component it is
                    for (int i=0; i<num_match; i++)
                    {
                        // Goes through all components to find what the connection is
                        int success = border_check(locations, k, j, i);
                        if (success == 1)
                        {
                            // Store the current value of i
                            (*store_connections)[*successful_connect] = i;
                            // Increment the count of successful matches
                            (*successful_connect)++;
                        }
                    }
                }
            }
        } 
}

/*
This function will take in an array of connections and print the connections statement
*/
void print_conenctions(int successful_connect, int *store_connections, int i){
    printf("Component %d connected to ", i); 
    if (successful_connect == 0)
    {
        printf("nothing");
    }  
    else
    {
        // Loop through all the stored connections and print
        for (int l=0; l<successful_connect; l++)
        {
            if (l == 0)
            {
                printf("%d ", store_connections[l]);
            }
            // Only print a particular connection once
            else if (store_connections[l] != store_connections[l-1])
            {
                printf("%d ", store_connections[l]);
            }
        }
    }
    printf("\n");
}

/*
Uses part 2 cide to list all the components and their locations
Then checks for connections of each component.
*/
void connection_main(char *temp_filename, char *filename){
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
    // Num of templates in template file
    int number_of_templates = number_of_components(fp_bin);
    // Array that all the locations will be stored in
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
            match_points[x] = malloc(LOCATIONS*sizeof(int*));
            // Set initial values to, if these change, we know there is a match 
            match_points[x][0] = -1;
            match_points[x][1] = -1;
            // Call for function to find location of a match and store it in a 2d array
            int **location = find_template_in_bmp(thresholded_bmp, bmp_array, new_temp_array, match_points);
            // store location in array
            if (location[x][0] != -1)
            {
                //store the component number/type in the array with the position for sorting purposes
                location[x] = realloc(location[x], sizeof(int));
                location[x][2] = i;
                // store the location in a larger array which will hold all the locations of all matches
                store_location[num_match] = malloc(sizeof(int*));
                store_location[num_match] = location[x];
                num_match++;
            }
        }
    // Free the template arrays
    free_temp_array(temp_array, new_temp_array, TEMPLATE_XY);
    }
    // Print / List the components 
    print_wrap(num_match, store_location);

    // Start of part 3
    // Loop through to find connections for each particular component
    for (int i=0; i<num_match; i++)
    {
        // Create a component array
        int **component_grid = create_visited_grid(thresholded_bmp.height, thresholded_bmp.width);
        // Every template is labelled with a 2
        component_assign(num_match, store_location, component_grid);
        // Make the component that we are searching 0  
        zero_out_function(component_grid, store_location, i);
        // Create a visited grid
        int **visited = create_visited_grid(thresholded_bmp.height, thresholded_bmp.width);
        // Run DFS, which determines locations through comparison o the component grid
        dfs(bmp_array, visited, component_grid, store_location[i][0],store_location[i][1] , thresholded_bmp.height, thresholded_bmp.width);
        // Initialize the counter for successful matches
        int successful_connect = 0;
        // Allocate memory to store indices where success == 1
        int *store_connections = malloc(num_match * sizeof(int));
        // Check for connections, store in store_connections via pass by reference 
        check_connections(component_grid, store_location, num_match,thresholded_bmp.height, thresholded_bmp.width, &store_connections, &successful_connect);
        // Sort in in order of components
        qsort(store_connections, successful_connect, sizeof(int), compare_ints);
        // Print the connections
        print_conenctions(successful_connect, store_connections, i);
        // Free all data on heap 
        free(store_connections);
        free_grid(component_grid, thresholded_bmp.height);
        free_grid(visited, thresholded_bmp.height);         
    }
    // Free heap data
    free_2d_array_loc(store_location, num_match);
    free_2d_array_char(bmp_array, thresholded_bmp.height);
    free_bmp(thresholded_bmp);
    fclose(fp_bmp);
}
 
