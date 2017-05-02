#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char **argv)
{
   int i;
   int taskid, ntasks;
   int msg_tag = 0;
   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
   MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
   int msg = 0;


   if (taskid == 0){
      MPI_Send(&msg, 1, MPI_INT, 0, taskid+1, MPI_COMM_WORLD);
   }

   else if(taskid < ntasks-1) {
      MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      msg ++;
      MPI_Send(&msg, 1, MPI_INT, taskid+1, 0, MPI_COMM_WORLD);
   }

   else
   {
      MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
      printf("msg: %d", msg);
   }


   MPI_Finalize();
   return 0;
}
