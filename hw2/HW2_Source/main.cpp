#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include "parallel_sort.hpp"
#include "mpi.h"
#include <unistd.h>

bool is_vector_sorted( float* data, int length);

float* quick_sort(float *data, int length, MPI_Comm comm);

int main(int argc, char *argv[]) 
{
   if (argc < 2) 
   {
      std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
      return 1;
   }
   std::string filename = argv[1];

   MPI_Init(&argc, &argv);
   int myid, np;

   MPI_Comm_rank(MPI_COMM_WORLD, &myid);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   std::vector<float> init_array;

   if (myid == 0) 
   {
      std::ifstream inputFile(filename, std::ios::binary);
      if (!inputFile) {
         std::cerr << "Error opening file: " << filename << std::endl;
         return 1;
      }

      float value;
      while (inputFile.read(reinterpret_cast<char*>(&value), sizeof(float))) {
         init_array.push_back(value);
      }

      inputFile.close();

   }

   MPI_Barrier(MPI_COMM_WORLD);
   auto start = std::chrono::high_resolution_clock::now();

   float* sorted_array = parallel_quicksort(init_array.data(), init_array.size(), MPI_COMM_WORLD);
   // float* sorted_array = quick_sort(init_array.data(), init_array.size(), MPI_COMM_WORLD);

   MPI_Barrier(MPI_COMM_WORLD);
   auto end = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> elapsed = end - start;

   if(myid == 0)
   {
      std::cout << "Time: " << elapsed.count() << " seconds" << std::endl;
      bool is_sorted = is_vector_sorted(sorted_array, init_array.size());
      if (is_sorted)
      {
         std::cout << "The array is sorted" << std::endl;
      }
      else
      {
         std::cout << "The array is not sorted" << std::endl;
      }
   }

   MPI_Finalize();

   return 0;
}

bool is_vector_sorted(float* data, int length)
{
   for (int i = 0; i < length - 1; i++)
   {
      if (data[i] > data[i + 1])
      {
         return false;
      }
   }
   return true;
}

void quicksort(float* data, int left, int right) 
{
   int i = left, j = right;
   float pivot = data[(left + right) / 2];

   while (i <= j) {
      while (data[i] < pivot)
         i++;
      while (data[j] > pivot)
         j--;
      if (i <= j) {
         std::swap(data[i], data[j]);
         i++;
         j--;
      }
   }

   if (left < j)
      quicksort(data, left, j);
   if (i < right)
      quicksort(data, i, right);
}

float* quick_sort(float *data, int length, MPI_Comm comm)
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
      memcpy(sorted_data, data, length * sizeof(float));
      quicksort(sorted_data, 0, length - 1);
   }

   return sorted_data;
}