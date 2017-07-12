#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "omp.h"
#define N 100000000 //33554432 // 2 ^ 25

void computeparallelprefix(int *iplist, int *_pprefixsum, unsigned long size)
{
  int nthr, *z, *x = _pprefixsum;
  omp_set_num_threads(16);
  #pragma omp parallel
  {
    int i;
    #pragma omp single
    {
      nthr = omp_get_num_threads();
      z = malloc(sizeof(int)*nthr+1);
      z[0] = 0;
    }
    int tid = omp_get_thread_num();
    int sum = 0;
    #pragma omp for schedule(static) 
    for(i=0; i<size; i++) {
      sum += iplist[i];
      x[i] = sum;
    }
    z[tid+1] = sum;
    #pragma omp barrier

    int offset = 0;
    for(i=0; i<(tid+1); i++) {
        offset += z[i];
    }

    #pragma omp for schedule(static)
    for(i=0; i<size; i++) {
      x[i] += offset;
    }
  }
  free(z);
}

int main(void ) {

  int *iplist, *pprefixsum ;

  iplist = (int*) malloc(sizeof(int) * N);
  pprefixsum = (int*) malloc(sizeof(int) * N);

  int i;

  for(i=0; i<N; i++) iplist[i] = 1;
  //for(i=0; i<N; i++) printf("%d ", iplist[i]); printf("\n");

    double t_init, t_final;

  t_init=omp_get_wtime();
  computeparallelprefix(iplist, pprefixsum, N);

  t_final=omp_get_wtime();
  printf("%f\n", (t_final-t_init));

  printf("%d ", pprefixsum[N-1]); printf("\n");
  //for(i=0; i<N; i++) printf("%d ", (i+1)*(i+2)/2); printf("\n");
  return 0;
}