#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
   MPI_Init(&argc, &argv);

   int myid, numprocs;
   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);

   int x = myid;
   int sum = 10086;

   for(int id = 0 ; id < numprocs; id++)
   {
      if(myid == id)
      {
         std::cout << "Before Allreduce, processor " << myid << " has x = " << x << std::endl;
      }
      MPI_Barrier(MPI_COMM_WORLD);
   }

   MPI_Allreduce(&x, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

   for(int id = 0 ; id < numprocs; id++)
   {
      if(myid == id)
      {
         std::cout << "After Allreduce, processor " << myid << " has sum = " << sum << std::endl;
      }
      MPI_Barrier(MPI_COMM_WORLD);
   }
   MPI_Barrier(MPI_COMM_WORLD);

   MPI_Finalize();

   return 0;
}