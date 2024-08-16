/* Name: Angelica Vargas
*  Lab: 11
*  Assignment 2: Implement the Estimating pi algorithm
*/

#include <stdio.h>
#include <omp.h>

int main() {
    int n = 100;  // number of iterations
    double factor = 1.0;
    double sum = 0.0;
    int thread_count = 4;  //  number of threads

    #pragma omp parallel for num_threads(thread_count) reduction(+:sum)
    for (int k = 0; k < n; k++) {
        sum += factor / (2.0 * k + 1);
        factor = -factor;
    }

    double pi_estimate = 4.0 * sum;

    printf("Estimated value of pi: %.15f\n", pi_estimate);
    return 0;
}
