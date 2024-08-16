/* Name: Angelica Vargas
*  Lab: 11
*  Assignment 1: Implement the trapezoidal rule (omp parallel for)
*/

#include <stdio.h>
#include <omp.h>

double f(double x) {
    return x * x;
}

int main() {
    double a = 0.0;   // start of the interval
    double b = 2.0;   // end of the interval
    int n = 100;  // num of trapezoids
    int thread_count = 4; // num of threads

    double h = (b - a) / n;
    double approx = (f(a) + f(b)) / 2.0;

    #pragma omp parallel for num_threads(thread_count) reduction(+:approx)
    for (int i = 1; i <= n - 1; i++) {
        double x = a + i * h;
        approx += f(x);
    }

    approx = h * approx;

    printf("Approximate integral: %f\n", approx);

    return 0;
}
