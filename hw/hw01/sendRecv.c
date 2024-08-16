/*	Cpt S 411, Introduction to Parallel Computing
 *	School of Electrical Engineering and Computer Science
 *	
 *	Example code - Send receive test:
 *   	rank 1 sends to rank 0 (all other ranks sit idle)
 *   	For timing use of C gettimeofday() is recommended.
 * */

#include <stdio.h>
#include <mpi.h> 
#include <assert.h>
#include <sys/time.h>

int main(int argc,char *argv[]) 
{

	int rank, p, i = 0;
    struct timeval time1, time2;
	//unsigned long start, end;

	FILE * sendOutput = fopen("sendOutput.csv", "w");
	FILE * recvOutput = fopen("recvOutput.csv", "w");

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	for (i = 0; i < 10; i++) {

		printf("my rank=%d\n",rank);
		printf("Rank=%d: number of processes = %d\n", rank, p);

		assert(p >= 2);

		if(rank == 1) {

			int bytes = 1000; // num bytes to send in current
			int bytesToSend = 0; // bytes to send per iteration
			int bytesSent = 0; // bytes sent
			int maxBytes = 1000000; // 1 MB
			char sendBuf[1024] = { 0 };  // character array with zero
			int tSend, dest = 0;

				// does iterations 1, 2, 4, 8 and 16 until 1MB
				do {

					bytesSent = 0;
					gettimeofday(&time1, NULL);

					while (bytesSent < bytes) {

						if (bytes - bytesSent >= 1024) {
							bytesToSend = 1024;
						} else {

							bytesToSend = bytes - bytesSent;
						}

						MPI_Send(sendBuf, bytesToSend, MPI_CHAR, dest, 0, MPI_COMM_WORLD);
						bytesSent += bytesToSend;
					}
						gettimeofday(&time2, NULL);
						tSend = (time2.tv_usec - time1.tv_usec);
						printf("%d bytes sent in %d microseconds\n", bytesSent, tSend);
						fprintf(sendOutput, "%d,%d\n", tSend, bytesSent);

						bytes *= 2;

					} while (bytes < maxBytes);

				} else if (rank == 0) {
					
					MPI_Status status;  // communication to receive status - completes MPI

					int bytes = 1000; // num bytes to send in current
					int bytesToRecv = 0; // bytes receive per iteration
					int bytesRecvd = 0; // bytes received
					int maxBytes = 1000000; // 1 MB
					char recvBuf[1024] = { 0 };
					int tRecv, dest = 0;

				// does iterations 1, 2, 4, 8 and 16 until 1MB
					do { 
						bytesRecvd = 0;

						gettimeofday(&time1, NULL);

						while (bytesRecvd < bytes) {
							if (bytes - bytesRecvd >= 1024) {
								bytesToRecv = 1024;
							}
							else { // does not execute when using powers of 2
								bytesToRecv = bytes - bytesRecvd;
							}

							MPI_Recv(recvBuf, bytesToRecv, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
							bytesRecvd += bytesToRecv;
						}

						gettimeofday(&time2, NULL);
						
						tRecv = (time2.tv_usec - time1.tv_usec);
						printf("%d bytes received in %d microseconds\n", bytesRecvd, tRecv);
						fprintf(recvOutput, "%d,%d\n", tRecv, bytesRecvd);

						bytes *= 2;
					} while (bytes < maxBytes);
				}
	}
	MPI_Finalize();
	
	fclose(sendOutput);
	fclose(recvOutput);
}