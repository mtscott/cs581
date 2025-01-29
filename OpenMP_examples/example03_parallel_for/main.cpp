#include <stdlib.h>
#include <iostream>
#include <omp.h>
#include <chrono>
#include <cmath>

using namespace std;

#define VEC_LENGTH 1000000
#define NUM_TESTS 100

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cout << "Usage: " << argv[0] << " <num_threads>" << endl;
        return 1;
    }

    int num_threads = atoi(argv[1]);
    omp_set_num_threads(num_threads);

    double *a = new double[VEC_LENGTH];
    double *b = new double[VEC_LENGTH];
    double *c = new double[VEC_LENGTH];

    for(int i = 0; i < VEC_LENGTH; i++)
    {
        a[i] = i;
        b[i] = -i;
    }

    auto start = chrono::high_resolution_clock::now();

    for(int t = 0; t < NUM_TESTS; t++)
    {
        #pragma omp parallel for
        for(int i = 0; i < VEC_LENGTH; i++)
        {
            c[i] = a[i] + b[i];
        }
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Elapsed time: " << elapsed.count() << " s" << endl;

    for(int i = 0; i < VEC_LENGTH; i++)
    {
        if(fabs(c[i]) > 1e-10)
        {
            cout << "Error at position " << i << " c[i] = " << c[i] << endl;
            return 1;
        }
    }

    delete[] a;
    delete[] b;
    delete[] c;

    return 0;
}