/*
*
* d) Prueba el planificador auto.
*
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h> 


#define N 10
#define nthreads 4
 
int main (){
  int thread,i;
  omp_set_num_threads(nthreads);
  #pragma omp parallel private(thread, i)
  {
      thread = omp_get_thread_num();
 
      #pragma omp for schedule(auto)
      for (i = 0 ; i < N; i++){
        printf("\nSoy el hilo: %d ejecuto la iteracion %d\n", thread, i);
      }
 
  }
}