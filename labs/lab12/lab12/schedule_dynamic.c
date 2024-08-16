#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

int main() {
    int num_threads = 1000;
    int n = 100;
    double start, end;
    
    start = omp_get_wtime();
    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp for schedule(dynamic)
        for (int i = 0; i < n; i++) {
            int thread_id = omp_get_thread_num();
            int c=0;
            for(int j = 0; j < n; j++){
                c+=sin(j);
            }
            printf(" Thread %*d: Iteration %d\n",2*thread_id, thread_id, i);
        }
    }

    end = omp_get_wtime();
    printf("Time: %f\n", end - start); 


    return 0;
}
