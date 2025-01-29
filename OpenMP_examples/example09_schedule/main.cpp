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
    int num_threads = 4;
    omp_set_num_threads(num_threads);

    #pragma omp parallel for schedule(static, 2)
    //#pragma omp parallel for schedule(dynamic, 1)
    for(int i = 0; i < 10; i ++)
    {
        printf("Thread %d, i = %d\n", omp_get_thread_num(), i);
    }

    return 0;
}