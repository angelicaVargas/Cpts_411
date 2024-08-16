/*
 * Distributed under terms of the GNU LGPL3 license.
 */

#include "mpi.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int number;

    // process 0 sends the number to process 1
    if (world_rank == 0)
    {
        number = 10;  
        // number is differennt on both if statements
        //runs on two diff processes
        for(int i = 1; i < world_size; i++)
        MPI_Send(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        // MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // printf("Process %d received number %d from process 0\n", world_rank, number);
    }
    else if (world_rank == 1)
    {
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process %d received number %d from process 0\n", world_rank, number);
        // number += 10;
        // MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}