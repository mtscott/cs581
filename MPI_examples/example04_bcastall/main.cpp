#include <mpi.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv) {
   MPI_Init(&argc, &argv);

   int myid, numprocs;
   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);

   if(myid == 0)
   {
      std::cout << "Number of processors: " << numprocs << std::endl;
   }

   int *x = new int[numprocs];
   for(int i = 0; i < numprocs; i++)
   {
      x[i] = i + myid * numprocs;
   }

   for(int id = 0 ; id < numprocs; id++)
   {
      if(myid == id)
      {
         std::cout << "Before all-to-all broadcast, processor " << myid << " has x = ";
         for(int i = 0; i < numprocs; i++)
         {
            std::cout << x[i] << " ";
         }
         std::cout << std::endl;
      }
      MPI_Barrier(MPI_COMM_WORLD);
   }

   MPI_Alltoall(MPI_IN_PLACE, 1, MPI_INT, x, 1, MPI_INT, MPI_COMM_WORLD);

   for(int id = 0 ; id < numprocs; id++)
   {
      if(myid == id)
      {
         std::cout << "After all-to-all broadcast, processor " << myid << " has x = ";
         for(int i = 0; i < numprocs; i++)
         {
            std::cout << x[i] << " ";
         }
         std::cout << std::endl;
      }
      MPI_Barrier(MPI_COMM_WORLD);
   }

   MPI_Finalize();
   return 0;
}