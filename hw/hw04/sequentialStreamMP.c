/*
 * Name: Angelica Vargas
 * Assignment 4: Implement and parallelize the Stream Compaction algorithm using OpenMP
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void printArray(const char* label, const int* array, int size) {
    printf("%s: ", label);
    for(int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int sequentialStreamCompact(const int* input, int* output, int size) {
    int count = 0, nonZero = 0;
    int* index = (int*)malloc(size * sizeof(int));
    int* prefixSum = (int*)malloc(size * sizeof(int));

    // function that if divisible by 2, then converts value into 1 or 0
    #pragma omp parallel for
    for(int i = 0; i < size; i++) {
        index[i] = (input[i] % 2 == 0) ? 1 : 0;
    }

    // function that adds the prefixes
    prefixSum[0] = index[0];
    #pragma omp parallel for
    for(int j = 1; j < size; j++) {
        prefixSum[j] = prefixSum[j-1] + index[j];
    }

    // function to have prefix values copied into the new array
    #pragma omp parallel for
    for(int k = 0; k < size; k++) {
        if(index[k]) {
            int value = input[k];
            prefixSum[k] = value ;
            output[nonZero] = value;  // Use the input value directly
            nonZero++;
        }
    }

    free(index);
    free(prefixSum);

    return nonZero;
}

int main() {
    int size = 10;
    int input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int* output = (int*)malloc(size * sizeof(int));

    double start_time = omp_get_wtime();
    int compactedSize = sequentialStreamCompact(input, output, size);
    double end_time = omp_get_wtime();

    printArray("First: ", input, size);
    printArray("Compacted: ", output, compactedSize);

    printf("Execution time: %f seconds \n", end_time - start_time);
    free(output);

    return 0;
}
