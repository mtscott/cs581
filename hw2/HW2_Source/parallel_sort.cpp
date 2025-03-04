#include "parallel_sort.hpp"

// TODO: Your new header file here

// TODO: End of your new header file

// TODO: Your helper functions here

// int binarySearch(std::vector& arr, int target, int low, int high){
//    while (low <= high){
//       int mid = low + (high - low) / 2;
//       if (arr[mid] == target){
//          return mid;
//       } else if (arr[mid] < target){
//          low = mid + 1;
//       } else {
//          high = mid - 1;
//       }
//    }
//    return low;
// }



// int splittingVec(){
//     const std::vector<int> intList{1, 3, 5, 2};
//     int num_samples = 3; // Number of random samples to extract

//     // Seed for random number generation
//     std::random_device rd;
//     std::mt19927 gen(rd());

//     // Ensure the number of samples does not exceed the list size
//     num_samples = std::min(num_samples, (int)my_list.size());

//     // Randomly sample elements
//     std::vector<int> samples(num_samples);
//     std::sample(my_list.begin(), my_list.end(), samples.begin(), num_samples, gen);

//     // Print the sampled elements
//     std::cout << "Randomly sampled elements: ";
//     for (int sample : samples) {
//         std::cout << sample << " ";
//     }
//     std::cout << std::endl;

//     return 0;
// }

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
   int displs[np];
   int remainder = length % np;
   

   for (int i = 0; i < np; i++){  
      if (i < remainder){
         sendcounts[i] = length / np + 1; // Do its share and takes care of the remainders
      } else {
         sendcounts[i] = length / np;
      }
   }
   for (int i = 0; i < np; i++){
      if (i == 0){
         displs[i] = 0;
      } else {
         displs[i] = displs[i - 1] + sendcounts[i - 1];
      }
   }

   // if(myid == 0)
   // {  
   //    std::cout << "Before Scatterv, processor " << myid << " has x = ";
   //    for(int i = 0; i < length; i++)
   //    {
   //       std::cout << data[i] << " ";
   //    }
   //    std::cout << std::endl;
   // }
   MPI_Barrier(comm);
   
   // Broadcast the sendcounts and displs to all processors
   MPI_Bcast(sendcounts, np, MPI_INT, 0, comm);

   // Scatter the data
   float *y = new float[sendcounts[myid]];
   MPI_Scatterv(data, sendcounts, displs, MPI_FLOAT, y, sendcounts[myid], MPI_FLOAT, 0, comm);
   

   // for(int id = 0 ; id < np; id++)
   // {
   //    if(myid == id)
   //    {
   //       std::cout << "After Scatterv, processor " << myid << " has y["<<sendcounts[myid]<<"] = ";
   //       for(int i = 0; i < sendcounts[myid]; i++)
   //       {
   //          std::cout << y[i] << " ";
   //       }
   //       std::cout << std::endl;
   //    }
   //    MPI_Barrier(comm);
   // }

   // Assuming the data is already random so we can select the first k elements
   int k = 5; // selection parameter
   float *samples = new float[np*k];
   float *sortingVec = new float[np+1];
   for(int i = 0; i < np; i++){  
      if(myid == i){
         if (sendcounts[myid] < k){
            k = sendcounts[myid];
            std::cout << "Warning: Processor " << myid << " has less than k samples" << std::endl;
            MPI_Abort(comm, EXIT_FAILURE);
         }
         // std::cout << "Processor " << myid << " has samples = ";
         for(int j = 0; j < k; j++){
            samples[j] = y[j];
            // std::cout << samples[j] << " ";
         }
         // std::cout << std::endl;
         
      }
      MPI_Barrier(comm);
   }


   // for(int id = 0 ; id < np; id++)
   // {
   //    if(myid == id)
   //    {
   //       std::cout << "Before Gather, processor " << myid << " has x = ";
   //       for(int i = 0; i < np; i++){
   //          for (int j = 0; j < k; j++){
   //             std::cout << samples[j] << " ";
   //          }
   //       }
   //       std::cout << std::endl;
   //    }
   //    MPI_Barrier(comm);
   // }

   MPI_Gather(samples, k, MPI_FLOAT, samples, k, MPI_FLOAT, 0, comm);

   // for(int id = 0 ; id < np; id++)
   // {
   //    if(myid == id)
   //    {
   //       std::cout << "After Gather, processor " << myid << " has x = ";
   //       for(int i = 0; i < np * k; i++){
   //          std::cout << samples[i] << " ";
   //       }
   //       std::cout << std::endl;
   //    }
   //    MPI_Barrier(comm);
   // }

   // Construct the splitting vector
   std::sort(samples, samples + np*k);
   if ( myid == 0){
      // std::cout << "Sorted sameples = ";
      // for (int i = 0; i < np*k; i++){
      //    std::cout << samples[i] << " ";
      // }
      // std::cout << std::endl;
      // std::cout << "Sorted sampling vector = ";
      // std::cout << sortingVec[0] << " ";
      for(int i = 1; i < np; i++){
         sortingVec[i] = (samples[i*k-1] + samples[i*k])/2;
      //    std::cout << sortingVec[i] << " ";
      }
      sortingVec[np] = 1.0f;
      // std::cout << sortingVec[np] << std::endl;
   }
   // std::cout << std::endl;

   MPI_Bcast(sortingVec, np+1, MPI_FLOAT, 0, comm);

   // for(int id = 0 ; id < np; id++)
   // {
   //    if(myid == id)
   //    {
   //       std::cout << "After Bcast, processor " << myid << " has sorting vec = ";
   //       for(int i = 0; i < np+1; i++){
   //          std::cout << sortingVec[i] << " ";
   //       }
   //       std::cout << std::endl;
   //    }
   //    MPI_Barrier(comm);
   // }

   // for (int i = 0; i < np; i++){
   //    if (myid == i){
   //       std::cout << "Processor " << myid << " has sendcounts = " << sendcounts[i] << std::endl;
   //    }
   //    std::cout << std::endl;
   // }

   std::vector<float> dataBinned;
   std::vector<std::vector<float>> sendBuff(np);

   for(int j = 0; j < np; j++){
      for(int i = 0; i < sendcounts[j]; i++){
         if (y[i] >= sortingVec[j] && y[i] < sortingVec[j+1]){
            if (myid == j){
               dataBinned.push_back(y[i]);
            } else {
               sendBuff[j].push_back(y[i]);
            }
            // break;
         }
      }
   }

   // for (int i = 0; i < np; i++){
   //    if (myid == i){
   //       std::cout << "Processor " << myid << " has dataBinned = ";
   //       for (int j = 0; j < dataBinned.size(); j++){
   //          std::cout << dataBinned[j] << " ";
   //       }
   //       std::cout << std::endl;
      
      
   //       std::cout << "Processor " << myid << " has dataBinned size = ";
   //       std::cout << dataBinned.size() << std::endl;
   //    }
   //    std::cout << std::endl;
   // }


   std::vector<int> sendCounts(np);
   std::vector<int> sendDispls(np);
   std::vector<int> recvCounts(np);
   std::vector<int> recvDispls(np);

   
   for (int i = 0; i < np; i++){
      sendCounts[i] = sendBuff[i].size();
   }
   // std::cout << "Send Buff = ";
   // for (int i = 0; i < np; i++){
   //    if (myid != i){
   //       std::cout << "Processor " << myid << " has sendBuff = ";
   //       std::cout << sendBuff[i].size() << std::endl;
   //    }
   // }
   // std::cout << std::endl;
   

   MPI_Alltoall(sendCounts.data(), 1, MPI_INT, recvCounts.data(), 1, MPI_INT, comm);

   // for(int i = 0; i < np; i++){
   //    if (myid == i){
   //       std::cout << "Processor " << myid << " has recvCounts = ";
   //       for (int j = 0; j < np; j++){
   //          std::cout << recvCounts[j] << " ";
   //       }
   //       std::cout << std::endl;
   //    }
   //    MPI_Barrier(comm);
   // }

   int totalRecv = 0;
   for (int i = 0; i < np; i++){
      recvDispls[i] = totalRecv;
      totalRecv += recvCounts[i];
   }

   std::vector<float> sendData;
   for (int i = 0; i < np; i++){
      sendDispls[i] = sendData.size();
      sendData.insert(sendData.end(), sendBuff[i].begin(), sendBuff[i].end());
   }

   // for (int i = 0; i < np; i++){
   //    if (myid == i){
   //       std::cout << "Processor " << myid << " has sendData = ";
   //       for (int j = 0; j < sendData.size(); j++){
   //          std::cout << sendData[j] << " ";
   //       }
   //       std::cout << std::endl;
   //    }
   //    MPI_Barrier(comm);
   // }

   std::vector<float> recvData(totalRecv);

   MPI_Alltoallv(sendData.data(), sendCounts.data(), sendDispls.data(), MPI_FLOAT, recvData.data(), recvCounts.data(), recvDispls.data(), MPI_FLOAT, comm);

   // for(int id = 0 ; id < np; id++)
   // {
   //    if(myid == id)
   //    {
   //       std::cout << "After Alltoallv, processor " << myid << " has recvData = ";
   //       for(int i = 0; i < recvData.size(); i++){
   //          std::cout << recvData[i] << " ";
   //       }
   //       std::cout << std::endl;
   //    }
   //    MPI_Barrier(comm);
   // }

   // Add recieved data to the binned data
   dataBinned.insert(dataBinned.end(), recvData.begin(), recvData.end());

   for (int i = 0; i < np; i++){
      if (myid == i){
         std::cout << "Number of elements processor " << myid << " has:\t" << dataBinned.size() << std::endl;
      }
   }
   // std::cout << std::endl;
   
   // Sort the binned data
   std::sort(dataBinned.begin(), dataBinned.end());

   // for (int i = 0; i < np; i++){
   //    if (myid == i){
   //       std::cout << "Processor " << myid << " has " << "the sorted elements = ";
   //       for (int j = 0; j < dataBinned.size(); j++){
   //          std::cout << dataBinned[j] << " ";
   //       }
   //    }
   //    std::cout << std::endl;
   // }
   

   // Gather all the sorted data to the root processor
   int dataBinnedSize = dataBinned.size();
   int* recvcounts = NULL;
   int* sdispls = NULL;
   
   if (myid == 0) {
      recvcounts = new int[np];
      sdispls = new int[np];
   }

   if (myid == 0) {
      recvcounts = new int[np];
      sdispls = new int[np];
   }

   MPI_Gather(&dataBinnedSize, 1, MPI_INT, recvcounts, 1, MPI_INT, 0, comm);

   if (myid == 0) {
      sdispls[0] = 0;
      for (int i = 1; i < np; i++) {
         sdispls[i] = sdispls[i - 1] + recvcounts[i - 1];
      }
   }

   // Gather sorted data from each processor
   if (myid == 0) {
      sorted_data = new float[length];
   }

   MPI_Gatherv(dataBinned.data(), dataBinnedSize, MPI_FLOAT, sorted_data, recvcounts, sdispls, MPI_FLOAT, 0, comm);

   // TODO: End of your implementation

   
   // if (myid == 0){
   //    std::cout << "Processor " << myid << " has " << "the sorted elements = ";
   //    for (int j = 0; j < length; j++){
   //       std::cout << sorted_data[j] << " ";
   //    }
   // }
   // std::cout << std::endl;

   return sorted_data;
}