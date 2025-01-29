#include <stdlib.h>
#include <iostream>
#include <omp.h>
#include <chrono>
#include <cmath>
#include <stdio.h>

using namespace std;

#define VEC_LENGTH 100

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cout << "Usage: " << argv[0] << " <num_threads>" << endl;
        return 1;
    }

    int num_threads = atoi(argv[1]);
    omp_set_num_threads(num_threads);

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            printf("Section 1 on thread %d\n", omp_get_thread_num());
        }
        #pragma omp section
        {
            printf("Section 2 on thread %d\n", omp_get_thread_num());
        }
        #pragma omp section
        {
            printf("Section 3 on thread %d\n", omp_get_thread_num());
        }
    }

    return 0;
}