# pcb_quality_control

Code Description

The mian.c file consists of functions that are defined across many modules. All files are compiled into an executable, pcb_check.

The program is based upon the interpretation of files, which are of binary and bmp format. The file names are passed through via the command line. Holistaclly, the task requires the program to extract specific bits from these files and print out the data found in an orderly fashion. 

The main function consists of 3 if / else-if blocks, which determine the particular mode the user has selected.

If the use has selected: 

        Mode "t": An index is provided in the command line. If valid, the index represents a component in the template file. The program extracts the necessary binary data for that particular template, stores it in an arrray and then prints it out in the correct manor.

        Mode "l": In this mode the program will take in an image of .bmp format. The PCB board image contains particular components which are found in the template file. The data from the bmp file is thesholded and parsed through many functions until it is reorganised into an array. This array has 0's and 1's that reflect the white and black from thresholded image. Using that array, the program iterates over all the components from the template file, storing any matches and their locations. The matches are found using a series of nested for loops (sliding window method). The matches are then printed in order starting from the lowest row. 

        Mode "c": This mode builds upon the previous one. Using the same process to find the the locations of each component on the pcb board. It then continues on to see which components have electrical connections. A for loop is used to iterate over each component and search for it's connections. The search is done through the implementation of a Depth First Search (DFS) function on the 2d binary array. Another identically sized array, component_grid is created to keep track of when a component is hit during the DFS, it then marks on component_grid the connection location and returns out of that particular search path. This process enables the program to track all the connections for each individual component. The data is then printed in a row-first order of each component and it's connections.

This is the core flow of execution for pcb_check.  If there are any issues with file reading or incorrect command line arguments, the program will return and alert the user of the error. 
