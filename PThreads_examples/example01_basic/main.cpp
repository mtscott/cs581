#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

void* printHello(void* threadid) {
    //long tid;
    //tid = (long)threadid;
    int tid;
    tid = *(int*)threadid;
    //sleep(tid+1);
    cout << "Hello World from thread " << tid << endl;
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    const int NUM_THREADS = 4;
    pthread_t threads[NUM_THREADS];
    int rc;
    int i;

    for(i = 0; i < NUM_THREADS; i++) {
        cout << "main() : creating thread, " << i << endl;
        //rc = pthread_create(&threads[i], NULL, printHello, reinterpret_cast<void*>(i));
        rc = pthread_create(&threads[i], NULL, printHello, reinterpret_cast<void*>(&i));
        if (rc) {
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }

    // Wait for all threads to complete
    for(i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}