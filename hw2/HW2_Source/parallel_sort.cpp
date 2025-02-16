#include "parallel_sort.hpp"

// TODO: Your new header file here

// TODO: End of your new header file

// TODO: Your helper functions here

// TODO: End of your helper functions

float* parallel_quicksort(float* data, int length, MPI_Comm comm)
{
   MPI_Comm new_comm;
   MPI_Comm_dup(comm, &new_comm);

   int myid, np;
   MPI_Comm_rank(new_comm, &myid);
   MPI_Comm_size(new_comm, &np);
   float* sorted_data = NULL;

   if(myid == 0)
   {
      sorted_data = new float[length];
   }

   // TODO: Your implementation here

   // TODO: End of your implementation

   return sorted_data;
}