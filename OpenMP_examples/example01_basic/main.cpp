#include <stdlib.h>
#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;

int main(int argc, char** argv)
{
    #pragma omp parallel
    {
        #pragma omp critical
        {
            cout<< "Hello World from thread " << omp_get_thread_num() << endl;
        }
    }
    return 0;
}