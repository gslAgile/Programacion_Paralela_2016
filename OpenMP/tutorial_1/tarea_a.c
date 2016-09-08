#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
 
int main (int argc, char *argv[]){
 
int nthreads;
int thread;

if(argc > 1)
{
	nthreads = atoi(argv[1]); // convertimos el numero char en int
}
else
	nthreads = 10;

omp_set_num_threads(nthreads);
 
#pragma omp parallel private(nthreads, thread)
  {
 
  thread = omp_get_thread_num(); // funcion de omp.h que retorna el numero id de hilo
 
  nthreads = omp_get_num_threads(); // funcion de omp.h que retorna la cantidad de hilos

  /* Mostramos resultados por stdout*/
  printf("\nHola Mundo soy el hilo =%d de %d que somos en total.\n", thread, nthreads);
 
  }

  printf("\n");

  return 0;
 
}