#include <stdio.h> 
#include <stdlib.h>
#include <omp.h>
#include <math.h>

double f(double x) {
    return sqrt(sin(x)*sin(x)); 
}

void Trap(double a, double b, int n, double* global_result_p);

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Usage: %s <a> <b> <n> <num_threads>\n", argv[0]);
        return 1;
    }

    double a = atof(argv[1]);
    double b = atof(argv[2]);
    int n = atoi(argv[3]);
    int num_threads = atoi(argv[4]);

    double h = (b - a) / n;
    double global_result = 0.0;

    omp_set_num_threads(num_threads);
    
    double start_time = omp_get_wtime();
    Trap(a, b, n, &global_result);
    double end_time = omp_get_wtime();

    double elapsed_time = end_time - start_time;
    printf("Elapsed time: %lf seconds\n", elapsed_time);
    printf("Result: %lf\n", global_result);

    return 0;
}

void Trap(double a, double b, int n, double* global_result_p) {
    double h = (b - a) / n;
    int thread_count = omp_get_num_threads();
    double local_result = 0.0;

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int local_n = n / thread_count;
        double local_a = a + tid * local_n * h;
        double local_b = local_a + local_n * h;

        double my_result = (f(local_a) + f(local_b)) / 2.0;
        for (int i = 1; i < local_n - 1; i++) {
            double x = local_a + i * h;
            my_result += f(x);
        }
        my_result *= h;

        #pragma omp critical
        *global_result_p += my_result;
    }
}


