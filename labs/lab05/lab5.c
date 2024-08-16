#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Define the total number of elements and the size of each segment.
    int total_elements = 20*20; 
    int elements_per_process = total_elements / size;

   
    int *rbuf = (int *)malloc(elements_per_process * sizeof(int));  // per processor: 100
    int *sendbuf = (int *)malloc(total_elements * sizeof(int));  // total: 400
    srand((unsigned) time(NULL));

    int *newbuf = (int *)malloc(total_elements * sizeof(int));
   
    if (rank == 0) {
        // Allocate memory for the data on the root process (rank 0).
        // with random values
          for (int i = 0; i < total_elements; i++) {
            sendbuf[i] = i; //rand() % 100;
        }

        MPI_Scatter(sendbuf, elements_per_process, MPI_INT, rbuf, elements_per_process, MPI_INT, 0, MPI_COMM_WORLD); 
        printf("process %d receives %d", rank, rbuf[0]);
       // Scatter the data from the root process to all processes.
        // Perform computations on the local_data array
        for (int i = 0; i < elements_per_process; i++) {
            rbuf[i] += 1;
        }

        //Gather the results back to the root process:
        // Optional:Print the local data for each process.
        MPI_Gather(rbuf, elements_per_process, MPI_INT, newbuf, elements_per_process, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Synchronize all processes
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        //Process or print the gathered_data (e.g., print the results)
        printf("Gathered Data:\n");
        for (int i = 0; i < total_elements; i++) {
            printf("%d ", newbuf[i]);
        }
        printf("\n");
    }
    //Free allocated memory
    free(rbuf);
    free(sendbuf);
    free(newbuf);

    MPI_Finalize();

    return 0;
}
