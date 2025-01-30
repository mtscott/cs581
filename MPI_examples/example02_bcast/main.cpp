#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
   MPI_Init(&argc, &argv);

   int myid, numprocs;
   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);

   int x = myid;

   for(int id = 0 ; id < numprocs; id++)
   {
      if(myid == id)
      {
         std::cout << "Before Bcast, processor " << myid << " has x = " << x << std::endl;
      }
      MPI_Barrier(MPI_COMM_WORLD);
   }

   MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);

   for(int id = 0 ; id < numprocs; id++)
   {
      if(myid == id)
      {
         std::cout << "After Bcast, processor " << myid << " has x = " << x << std::endl;
      }
      MPI_Barrier(MPI_COMM_WORLD);
   }

   MPI_Finalize();
   return 0;
}