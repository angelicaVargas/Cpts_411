#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int row = 4;
int col = 4;

// separates the numbers in each column and row
void row_line() {
    printf("\n");
    for (int i = 0; i < col; i++) {
        printf(" -----");
    }
    printf("\n");
}

// returns the count of alive neighbors
int count_live_neighbour_cell(int a[row][col], int r, int c) {
    int i, j, count = 0;
    for (i = r - 1; i <= r + 1; i++) {
        for (j = c - 1; j <= c + 1; j++) {
            if ((i == r && j == c) || (i < 0 || j < 0) || (i >= row || j >= col)) {
                continue;
            }
            if (a[i][j] == 1) {
                count++;
            }
        }
    }
    return count;
}

int main(int argc, char *argv[]) {
    int rank, size;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int a[row][col], b[row][col];
    int i, j, t;
    int T = 5;
    int neighbour_live_cell;
    double start_time, end_time;

    if (rank == 0) {
        // create matrix with random values 0 and 1s
        for (i = 0; i < row; i++) {
            for (j = 0; j < col; j++) {
                a[i][j] = rand() % 2;
            }
        }

        // prints matrix
        printf("First matrix:");
        row_line();
        for (i = 0; i < row; i++) {
            printf(":");
            for (j = 0; j < col; j++) {
                printf("  %d  :", a[i][j]);
            }
            row_line();
        }

        start_time = MPI_Wtime(); // begins timing
    }

    MPI_Bcast(a, row * col, MPI_INT, 0, MPI_COMM_WORLD);

    for (t = 0; t < T; t++) {
        // parallel loop where cell value is updated
        for (i = rank; i < row; i += size) {
            for (j = 0; j < col; j++) {
                neighbour_live_cell = count_live_neighbour_cell(a, i, j);
                if (a[i][j] == 1) {
                    if (neighbour_live_cell < 3 || neighbour_live_cell > 5) {
                        b[i][j] = 0; // cell dies due to underpopulation or overpopulation
                    } else {
                        b[i][j] = 1; // cell lives on
                    }
                } else {
                    if (neighbour_live_cell >= 3 && neighbour_live_cell <= 5) {
                        b[i][j] = 1; // cell reproduction
                    } else {
                        b[i][j] = 0;
                    }
                }
            }
        }

        MPI_Gather(b, row * col, MPI_INT, a, row * col, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(a, row * col, MPI_INT, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        // prints new matrix
        printf("\nNew Matrix:");
        row_line();
        for (i = 0; i < row; i++) {
            printf(":");
            for (j = 0; j < col; j++) {
                printf("  %d  :", a[i][j]);
            }
            row_line();
        }

        end_time = MPI_Wtime(); // end timing

        printf("\n Time: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();

    return 0;
}
