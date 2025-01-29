#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

typedef struct {
  double      *a;
  double     sum;
  int     loclen;
  int     totlen;
} sumstr, *SumPtr;
sumstr SUMST;
#define NUMTHRDS 8
#define VECLEN 78
pthread_t callThd[NUMTHRDS];
pthread_mutex_t mutexsum;
void *sum_mtx(void *arg){
    int i, start, end, *blkNum, len ;
    double mysum, *x;
    blkNum = (int*)arg;
    len = SUMST.loclen;
    start = (*blkNum)*len;
    end = (*blkNum)<NUMTHRDS? start+len:SUMST.totlen;
    x = SUMST.a;
    mysum = 0;
    for (i=start; i<end ; i++){
        mysum += x[i];
    }
    pthread_mutex_lock (&mutexsum); 
    SUMST.sum += mysum;
    pthread_mutex_unlock (&mutexsum);
    printf(" -- Local sum in thread %d is %10.2f total: %10.2f\n ",
        *blkNum,mysum,SUMST.sum);
    printf("    len %d %d %d %5.2f \n",len,start,end,x[start]) ;
    pthread_exit((void*) 0);
}

int main (int argc, char *argv[]){
    int i, n = VECLEN;
    int *thrNum,  *status; 
    double *a;
    pthread_attr_t attr;
    a = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));
    thrNum  = (int*) malloc (NUMTHRDS*sizeof(int));
    for (i=0; i<n; i++) {
        a[i]=(double)i;
    }
    for (i=0; i<NUMTHRDS; i++){
        thrNum[i]= i; 
    }
    SUMST.totlen = n;
    SUMST.loclen = 1+(int) ((n-1)/NUMTHRDS); 
    SUMST.a = a; 
    SUMST.sum=0;
    pthread_mutex_init(&mutexsum, NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for(i=0;i<NUMTHRDS;i++) {
        pthread_create(&callThd[i],&attr,sum_mtx,(void *)&thrNum[i]);
    }
    pthread_attr_destroy(&attr); 
    for(i=0;i<NUMTHRDS;i++) {
        status = &thrNum[i];
        pthread_join( callThd[i], (void **)status);
        printf("  join number %d -- status %d \n",i,*status);
    }
    printf ("Total Sum in main thread =  %10.2f \n", SUMST.sum);
    free (a);  free (thrNum);
    pthread_mutex_destroy(&mutexsum);
    pthread_exit(NULL);
}  