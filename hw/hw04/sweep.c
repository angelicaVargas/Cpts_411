/*
 * Name: Angelica Vargas
 * Assignment 4: Implement upsweep and downsweep prefix sum algorithm using OpenMP
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void printArray(const char* label, int* array, int n) {
    printf("%s: ", label);
    for (int i = 0; i < n; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void upSweep(int* array, int n) {
    int i, j;
    int* output = (int*)malloc(n * sizeof(int));

    #pragma omp parallel for private(j)
    for (i = 0; i < (int)log2(n); i++) { // loops to log
        int var = (int)pow(2, i);  // calc of distance between elements

        #pragma omp parallel for
        for (j = 0; j < n; j++) {  // looop over each element of the array in parallel
            if (j < var) {  // measures if j has been updated 
                output[j] = array[j];  // copies value into output array
            } else {
                int index = j - var; // update index of element previous level
                output[j] = array[index] + array[j]; // update output value to the sum of the values
            }
        }

        // result back to the original array for the next iteration
        #pragma omp parallel for
        for (j = 0; j < n; j++) {
            array[j] = output[j];
        }
    }
    free(output);
}

void downSweep(int* array, int n) {
    int i, j;
    int* output = (int*)malloc(n * sizeof(int));

    output[n - 1] = array[n - 1]; // initialize  item of output

    #pragma omp parallel for private(j)
    for (int i = (int)log2(n); i > 0; i--) { // loop down starts from highest level, log
        int var = (int)pow(2, i);  // calc of distance between elements

        #pragma omp parallel for
        for (j = 0; j < n; j++) {  // looop over each element of the array in parallel
            if (j < var) {  // measures if j has been updated 
                output[j] = array[j];  // copies value into output array
            } else {
                int index = j - var; // update index of element previous level
                output[j] = array[index] + array[j]; // update output value to the sum of the values
            }
        }

        // result back to the original array for the next iteration
        #pragma omp parallel for
        for (j = 0; j < n; j++) {
            array[j] = output[j];
        }
    }
    free(output);
}

int main() {
    const int n = 5;
    int array[] = {1, 2, 3, 4, 5};

    printArray("Original array", array, n);

    upSweep(array, n);
    printArray("Up sweep array", array, n);

    downSweep(array, n);
    printArray("Down sweep", array, n);

    return 0;
}
