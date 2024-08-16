/* Name: Angelica Vargas
*  Lab: 14
*  Problem 1: Parallelizing Merge Sort
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// function to merge two sorted subarrays
void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // creating temp arrays
    int L[n1], R[n2];

    // copying data to temporary arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // merging the two arrays back into the original array
    i = 0; // first index of first subarray
    j = 0; // first index of sec subarray
    k = left; // first index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // copying the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // copying the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Parallel implementation of merge sort
void mergeSortParallel(int arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;

        // Create OpenMP tasks for the two subarrays
        #pragma omp task
        mergeSortParallel(arr, left, mid);

        #pragma omp task
        mergeSortParallel(arr, mid + 1, right);

        // making sure the two tasks are complete before merging
        #pragma omp taskwait

        // merging the two sorted subarrays
        merge(arr, left, mid, right);
    }
}

// parallelizes merge sort algorithm
void mergeSortParallelWrapper(int arr[], int size) {
    #pragma omp parallel 
    {
        #pragma omp single
        mergeSortParallel(arr, 0, size - 1);
    }
}

int main() {
    int arr[] = {15, 12, 11, 3, 4, 7}; // example array
    int size = sizeof(arr) / sizeof(arr[0]);

    // calling the parallelized merge sort
    mergeSortParallelWrapper(arr, size);

    // print array
    printf("Final array: \n");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
