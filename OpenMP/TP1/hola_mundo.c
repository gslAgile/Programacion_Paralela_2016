#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
 
int main (){
 
int nthreads;
int thread;
 
#pragma omp parallel private(nthreads, thread)
  {
 
  thread = omp_get_thread_num(); // funcion de omp.h que retorna el numero id de hilo
 
  nthreads = omp_get_num_threads(); // funcion de omp.h que retorna la cantidad de hilos

  /* Mostramos resultados por stdout*/
  printf("\nHola Mundo soy el hilo =%d de %d que somos en total.\n\n", thread, nthreads);
 
  }

  return 0;
 
}