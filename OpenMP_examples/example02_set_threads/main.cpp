#include <stdlib.h>
#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;

int main(int argc, char** argv)
{

    if(argc != 2)
    {
        cout << "Usage: " << argv[0] << " <num_threads>" << endl;
        return 1;
    }

    int num_threads = atoi(argv[1]);
    omp_set_num_threads(num_threads);

    #pragma omp parallel
    {
        #pragma omp critical
        {
            cout<< "Hello World from thread " << omp_get_thread_num() << endl;
        }
    }
    return 0;
}