#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define M 10 // number of rows
#define N 10 // number of columns
#define T 10  // number of time steps

// initialize the grid with randomized 0 or 1's
void initializeGrid(int localGrid[M][N]) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            localGrid[i][j] = rand() % 2;
        }
    }
}

// print the local grid
void printGrid(int localGrid[M][N]) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", localGrid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// count live neighbors of a cell
int countLiveNeighbors(int localGrid[M][N], int x, int y) {
    int liveNeighbors = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int newX = x + i;
            int newY = y + j;
            if (newX >= 0 && newX < M && newY >= 0 && newY < N) {
                if (localGrid[newX][newY] == 1) {
                    liveNeighbors++;
                }
            }
        }
    }
    return liveNeighbors - localGrid[x][y]; // subtract cell itself
}

int main(int argc, char** argv) {
    int rank, size;
    int localGrid[M][N];
    int nextLocalGrid[M][N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL));

    // local grid size
    int localM = M / size;

    // initializing local grid
    initializeGrid(localGrid);

    for (int t = 0; t < T; t++) {
        // Perform the Game of Life computation
        // Create a copy of the local grid for the next time step
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                int liveNeighbors = countLiveNeighbors(localGrid, i, j);
                if (localGrid[i][j] == 1) {
                    // Apply rules for live cells
                    if (liveNeighbors < 2 || liveNeighbors > 3) {
                        nextLocalGrid[i][j] = 0; // Cell dies
                    } else {
                        nextLocalGrid[i][j] = 1; // Cell survives
                    }
                } else {
                    // Apply rule for dead cells
                    if (liveNeighbors == 3) {
                        nextLocalGrid[i][j] = 1; // Cell becomes alive
                    } else {
                        nextLocalGrid[i][j] = 0; // Cell remains dead
                    }
                }
            }
        }

        // Synchronize processes
        MPI_Barrier(MPI_COMM_WORLD);

        // Update the local grid with the results for the next time step
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                localGrid[i][j] = nextLocalGrid[i][j];
            }
        }

        // Exchange information with neighboring processes
        // (You need to implement MPI communication here)
        if (rank > 0) {
            MPI_Send(localGrid[0], N, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
            MPI_Recv(localGrid[0], N, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        // Send the bottom row of the local grid to the process below
        if (rank < size - 1) {
            MPI_Send(localGrid[localM - 1], N, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
            MPI_Recv(localGrid[localM], N, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    
        // Output the local grid at each time step for debugging
        printf("Rank %d, Time Step %d:\n", rank, t);
        printGrid(localGrid);
    }

    // Gather results on the root process (Rank 0)
    // (You need to implement MPI_Gather here)

    if (rank == 0) {
        // Print the final grid on the root process
        printf("Final Grid:\n");
        printGrid(localGrid);
    }

    MPI_Finalize();
    return 0;
}
