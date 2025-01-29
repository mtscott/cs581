#include <stdlib.h>
#include <iostream>
#include <omp.h>
#include <chrono>
#include <cmath>

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

    double val;
    
    #pragma omp parallel for shared(val)
    for(int i = 0; i < 10000; i++)
    {
        #pragma omp critical
        {
            val = val + (double) i;
        }
    }

    cout << "val: " << val << endl;

    return 0;
}