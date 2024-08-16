/* Name: Angelica Vargas
*  Lab: 11
*  Assignment 3: Implement the Serial Odd-Even Transposition Sort
*/

#include <stdio.h>
#include <omp.h>

void oddEvenSort(int a[], int n, int thread_count) {
    int phase, i, tmp;

        #pragma omp parallel for num_threads(thread_count) default(none) shared(a, n) private(i, tmp, phase)
        for (int phase = 0; phase < n; phase++) {
            if (phase % 2 == 0) {
            for (int i = 1; i < n; i += 2) {
                if (a[i - 1] > a[i]) {
                    tmp = a[i];
                    a[i] = a[i-1];
                    a[i-1] = tmp;
                }
            }
        } else {
            for (int i = 1; i < n - 1; i += 2) {
                if (a[i] > a[i + 1]) {
                    tmp = a[i];
                    a[i] = a[i + 1];
                    a[i + 1] = tmp;
                }
            }
        }
    }
}

int main() {
    int n = 10;
    int a[] = {5, 2, 9, 1, 5, 6, 3, 2, 8, 7};
    int thread_count = 4;

    printf("Original array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    oddEvenSort(a, n, thread_count);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}
