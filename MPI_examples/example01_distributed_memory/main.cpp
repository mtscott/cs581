#include <mpi.h>
#include <iostream>
#include <unistd.h>
#include <omp.h>

int main(int argc, char** argv) {
   MPI_Init(&argc, &argv);

   int myid, numprocs;
   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);

   if(argc != 2)
   {
      omp_set_num_threads(2);
   }
   else
   {
      omp_set_num_threads(atoi(argv[1]));
   }
   if(myid == 0)
   {
      std::cout << "Number of processors: " << numprocs << std::endl;
      std::cout << "Number of threads: " << omp_get_max_threads() << std::endl;
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
         #pragma omp parallel
         {
            int tid = omp_get_thread_num();
            #pragma omp critical
            {
               std::cout << "Processor " << myid << " thread " << tid << " has x = ";
               for(int i = 0; i < numprocs; i++)
               {
                  std::cout << x[i] << " ";
               }
               std::cout << std::endl;
            }
         }
      }
      MPI_Barrier(MPI_COMM_WORLD);
   }

   sleep(1);
   
   for(int i = 0; i < numprocs; i++)
   {
      if(myid == i)
      {
         #pragma omp parallel
         {
            int tid = omp_get_thread_num();
            #pragma omp critical
            {
               std::cout << "x[0] on processor " << myid << " thread " << tid << " is " << x[0] << std::endl;
            }
         }
      }
      MPI_Barrier(MPI_COMM_WORLD);
   }

   MPI_Finalize();
   return 0;
}