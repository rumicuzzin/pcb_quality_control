#include <stdio.h>
#include <stdlib.h>

// This function is to sort bits of data in order of first row
// Comparator function for qsort
int compare(const void *a, const void *b) {
    int *loc1 = *(int **)a;
    int *loc2 = *(int **)b;
    if (loc1[0] != loc2[0]) { // Compare based on rows
        return loc1[0] - loc2[0];
    }
    return loc1[1] - loc2[1]; // If rows are equal, compare based on columns
}


// This function sorts an array in order of first type
int compare_types(const void *a, const void *b) {
    const int *ia = *(const int **)a;
    const int *ib = *(const int **)b;
    return ia[2] - ib[2];
}


// Comparison function for qsort - sorts in ascending order
int compare_ints(const void* a, const void* b) {
    // Cast from void* to int*
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0; // equal
}






