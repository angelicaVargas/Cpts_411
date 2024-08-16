#include <stdio.h>
#include <mpi.h>

#define N 1

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunk_size = N / size;
    int start = rank * chunk_size;
    int end = start + chunk_size;

    int A[N][N][N];
    int B[N][N][N];
    int C[N][N][N];

    // Initialize arrays A and B here

    MPI_Barrier(MPI_COMM_WORLD); // Synchronize all processes

    double start_time = MPI_Wtime();

    // Perform addition in parallel
    for (int i = start; i < end; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j][k] = A[i][j][k] + B[i][j][k];
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD); // Synchronize all processes

    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Execution time for %d processes: %f seconds\n", size, end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}