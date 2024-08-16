/*	Cpt S 411, Introduction to Parallel Computing
 *	School of Electrical Engineering and Computer Science
 *	
 *	Example code - Send receive test:
 *   	rank 1 sends to rank 0 (all other ranks sit idle)
 *   	For timing use of C gettimeofday() is recommended.
		this is a draft funcion for timing
		unsigned long utime(void)
		{
			struct timeval tv;
			unsigned long result = 0;

			gettimeofday(&tv, NULL);
			result += (tv.tv_sec * 1000000);
			result += tv.tv_usec;

			return result;
		}
 * */

#include <stdio.h>
#include <mpi.h> 
#include <assert.h>
#include <sys/time.h>

int main(int argc,char *argv[]) 
{

	int rank, p;
    struct timeval time1, time2;
	//unsigned long start, end;

	FILE * sendOutput = fopen("sendOutput.csv", "w");
	FILE * recvOutput = fopen("recvOutput.csv", "w");

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	printf("my rank=%d\n",rank);
	printf("Rank=%d: number of processes = %d\n", rank, p);

	assert(p >= 2);


	if(rank == 1) {
		int x = 10;
		int dest = 0;
        gettimeofday(&time1, NULL);
		//start = utime();
		MPI_Send(&x, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        gettimeofday(&time2, NULL);
        double tSend = (time2.tv_sec-time1.tv_sec) * 1000 + (time2.tv_usec-time1.tv_usec) / 1000;
		//end = utime();  
		// prepares value to send and takes the time

		printf("Rank=%d: sent message %d to rank %d; Send time %lf millisec\n", rank, x, dest, tSend);
	} else 

	if (rank == 0) {
		int y = 0;
		MPI_Status status;
		gettimeofday(&time1, NULL);
        //start = utime();
		MPI_Recv(&y, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		gettimeofday(&time2, NULL);
        //end = utime();
		double tRecv = (time2.tv_sec - time1.tv_sec) * 1000 + (time2.tv_usec - time1.tv_usec) / 1000;
		printf("Rank = %d: received message %d from rank %d; Recv time %lf millisec\n", rank, y, status.MPI_SOURCE, tRecv);
	}

	MPI_Finalize();
}