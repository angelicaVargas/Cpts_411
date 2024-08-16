#!/bin/sh

#To run this script, recommended use: "sbatch -N <numberofnodes> -n <number_of_processes> <path>/sub.sh"

# setting the max wallclock time after which the job will be killed;
# note: the format is hr:min:sec (default set here to 10 mins)
#SBATCH --time=00:20:00


#MPI helloworld example - this line is a comment
#this is the command to execute your parallel program

# Note that my helloworld resides on this path. You need to change the path where your executable resides
mpirun ~/lab6/lab6

# you can add more lines of commands here, if you wish.
