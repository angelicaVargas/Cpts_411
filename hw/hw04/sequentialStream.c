#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printArray(const char* label, const int* array, int size) {
    printf("%s: ", label);
    for(int i = 0; i <size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int sequentialStreamCompact(const int* input, int* output, int size) {
    int count = 0;

    for(int i = 0; i < size; i++) {
        if (input[i] % 2 == 0) {
            output[count++] = input[i];
        }
    }
    return count; //return the size of the compacted array
}

int main() {
    int size = 10;
    int input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int* output = (int*)malloc(size * sizeof(int));

    clock_t start = clock();

    int compactedSize = sequentialStreamCompact(input, output, size);

    clock_t end = clock(); // Record the end time

    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printArray("First: ", input, size);
    printArray("Compacted: ", output, compactedSize);

    printf("Time taken: %f seconds\n", cpu_time_used);
    
    free(output);

    return 0;
}