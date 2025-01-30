#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
   MPI_Init(&argc, &argv);

   int myid, numprocs;
   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);

   char processor_name[MPI_MAX_PROCESSOR_NAME];
   int name_len;
   MPI_Get_processor_name(processor_name, &name_len);

   std::cout << "Hello world from processor " << processor_name << ", rank " << myid << " out of " << numprocs << " processors" << std::endl;

   MPI_Finalize();
   return 0;
}