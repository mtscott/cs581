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

   int sendcounts[np];

   int moduler = length % np;
   for (int i = 0; i < np; i++)
   {  
      if (i < moduler)
      {
         sendcounts[i] = length / np + 1;
      } else {
         sendcounts[i] = length / np;
      }
   }
   std::cout << "Sendcounts: ";
   for (int i = 0; i < np; i++)
   {
      std::cout << sendcounts[i] << " ";
   }
   std::cout << std::endl;


   
   int displs[np];
   for (int i = 0; i < np; i++)
   {
      if (i == 0)
      {
         displs[i] = 0;
      } else {
         displs[i] = displs[i - 1] + sendcounts[i - 1];
      }
   }
   std::cout << "Displs: ";
   for (int i = 0; i < np; i++)
   {
      std::cout << displs[i] << " ";
   }
   std::cout << std::endl;

   if(myid == 0)
   {
      sorted_data = new float[length];
      

   }

   // TODO: Your implementation here
   // MPI_Scatterv(data, length, MPI_FLOAT, sorted_data, length, MPI_FLOAT, 0, new_comm);

   // for (int id = 0; id < np; id++)
   // {  
   //    if(myid == id)
   //    {
   //       std::cout << "After Scatterv, processor " << myid << " has y = ";
   //       for (int j = 0; j < length; j++)
   //       {
   //          std::cout << sorted_data[j] << " ";
   //       }
   //       std::cout << std::endl;
   //    }
   //    MPI_Barrier(new_comm);
   // }
   // MPI_Finalize();

   // TODO: End of your implementation

   return sorted_data;
}