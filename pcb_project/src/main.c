#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "component_display.h"
#include "component_list.h"
#include "list_and_connections.h"


int main(int argc, char **argv) 
{
    // If the user provides the incorrect number of arguments 
    if (argc != 4)
    {
        printf("Invalid arguements!\n");
        return 0;
    }

    // What mode we are testing in and filename
    char *mode = argv[1];
    char *temp_filename = argv[2];

    // If the user chooses template mode
    if (mode[0] == 't')
    {
        // Print out desired template image
        int index = atoi(argv[3]);
        int test = main_template_func(temp_filename, index);
        if (test == -1)
        {
            printf("Can't load template file\n");
        }
        else if (test == 0)
        {
            printf("Template index out of range\n");
        }

    }

    // If the user chooses list mode
    else if (mode[0] == 'l')
    {
        // list the components and thir locations
        char *bmp_filename = argv[3];
        int test = bmp_main_func(bmp_filename, temp_filename);
        if (test == -1)
        {
            printf("Can't load template file\n");
        }
    }

    // List components and connections mode
    else if (mode[0] == 'c')
    {
        char *bmp_filename = argv[3];
        connection_main(temp_filename, bmp_filename);
    }

    // IF the user gives an invalid input 
    else
    {
        printf("Invalid mode selected!\n");
    }
    
    return 0;
}

