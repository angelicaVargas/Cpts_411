/* Name: Angelica Vargas
*  Lab: 14
*  Problem 2: Parallel Task Execution with firstprivate
*/

#include <stdio.h>
#include <omp.h>


int parallelComputation(int array[], int n) {
    int result = 0;

    #pragma omp parallel
    {
        // single thread creates tasks
        #pragma omp single
        for (int i = 0; i < n; i++) {
            // each loop is task
            #pragma omp task firstprivate(i)
            {
                // perform some computatioin on array[i]
                int privateResult = array[i];

                // put result in private
                #pragma omp atomic
                result += privateResult;
            } 
        }
    }

    // waits 4 tasks to complete before returning result
    #pragma omp taskwait

    return result;
}


int main() {
    int array[] = {1, 2, 3, 4, 5}; // example array
    int n = sizeof(array) / sizeof(array[0]);

    int finalResult = parallelComputation(array, n);

    // printing final value
    printf("Final results: %d\n", finalResult);
    return 0;
}
