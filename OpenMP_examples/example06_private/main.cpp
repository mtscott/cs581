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

    const int n = 65536*128;
    const int m = n/num_threads;

    double *x = new double[n];
    double val = 0.0;
    for (int i = 0; i < n; i++) {
        x[i] = rand() / (double)RAND_MAX;
        val += x[i] * x[i];
    }
    val = sqrt(val);
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        x[i] /= val;
    }

    double t;
    double *tt = new double[num_threads];
    int i1, i2, i;
    
    auto start = chrono::high_resolution_clock::now();

    #pragma omp parallel for private(t, i1, i2, i)
    for (int it = 0; it < num_threads; it++) {   
        i1 = it*m;    
        i2 = i1+m;    
        if (i2 > n) {
            i2 = n;    
        }
        t = 0.0;    
        for ( i = i1; i <  i2; i++ ) {   
            t +=x[i]*x[i];    
        }
        tt[it] = t;
    }
    
    t = 0.0;   
    for (int it = 0; it< num_threads; it++) {  
        t += tt[it]; 
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Elapsed time: " << elapsed.count() << " s" << endl;

    cout << "ddot: " << t << endl;

    delete[] x;
    delete[] tt;

    return 0;
}