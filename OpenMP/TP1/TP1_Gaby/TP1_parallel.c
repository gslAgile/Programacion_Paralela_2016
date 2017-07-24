#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "omp.h"

/* Definicion de Macros*/
#define N 100000000 //33554432 // 2 ^ 25
#define porcion N/4 /* Porcion de iteraciones*/


/* Declaracion de funciones*/
void computeparallelprefix(int *iplist, int *_pprefixsum, unsigned long size);

/* Funcion principal
* Descripcion:
* @param int arg: numero de argumentos
* @param char *argv[]: puntero de la primera posicion de cada argumento recibido
*                      [0]: nombre del archivo
*                      [1]: primer argumento
*                      [...]: ...
*                      [n]: enesimo argumento
* @return: 0 (por defecto)
*/

int main(int arg, char * argv[]) {

/* Declaracion de variables*/
  int *iplist, *pprefixsum ;
  double t_init, t_final, t_procedural, t_paralelo, speedUp, porcentaje;
  int i, maxThrs=omp_get_max_threads(), nthreads=omp_get_num_threads();
  char sim=37; // simbolo de caracter equvalente a %

/* Inicializacion de valores*/
  omp_set_num_threads(nthreads);
  iplist = (int*) malloc(sizeof(int) * N);
  pprefixsum = (int*) malloc(sizeof(int) * N);

  for(i=0; i<N; i++) iplist[i] = 1;

  if(arg>1)
  {
    nthreads = atoi(argv[1]);
    printf("Parametro recibidos: %s (convertido a entero %i)\n",argv[1], nthreads);
    if(nthreads < 1)
    {
      printf("Parametro incorrecto. Se esablecen valores por defecto.\n");
      nthreads =1;
    }
    omp_set_num_threads(nthreads);
  }

  printf("Caracteristicas del sistema\n");
  printf("Numero de hilos disponibles en el equipo: %i\n",maxThrs);
  printf("Hilos usados para calculo paralelo: %i\n", nthreads);
  
  /*Calculo parallelo*/
  t_init=omp_get_wtime();
  computeparallelprefix(iplist, pprefixsum, N);

  t_final=omp_get_wtime();
  t_paralelo=(t_final-t_init);
  printf("Tiempo paralelo: %0.4lf segundos\n", t_paralelo);

  /*Calculo procedural*/
  omp_set_num_threads(1);
  t_init=omp_get_wtime();
  computeparallelprefix(iplist, pprefixsum, N);
  t_final=omp_get_wtime();
  t_procedural=(t_final-t_init);
  printf("Tiempo procedural: %0.4lf segundos\n", t_procedural);

  /*Calculo de speedUp (por Ley de Amdahl)*/
  speedUp = (t_procedural+t_paralelo)/(t_procedural+(t_paralelo)/nthreads);
  printf("SpeedUp segun Ley de Amdahl: %0.4lf\n", speedUp);

  /* Calculo de speedUp*/
  speedUp = t_procedural/t_paralelo;
  printf("SpeedUp: %0.4lf\n", speedUp);
  porcentaje = (speedUp*100)-100;
  printf("Velocidad paralela: %.2lf mas rapido aproximadamente\n", speedUp);
  printf("Porcentaje de mejora: %3.0lf%c aproximadamente\n\n", porcentaje, sim);

  printf("Ultimo dato obtenido: %d \n\n", pprefixsum[N-1]);
  //for(i=0; i<N; i++) printf("%d ", (i+1)*(i+2)/2); printf("\n");

  free(iplist);
  free(pprefixsum);
  return 0;
}

/* Implementacion de funciones*/


/*
* Descripcion:
* @param int *iplist: puntero a la primera posicion del vector contenedor de los valores de entrada.
* @param int *_pprefixsum: puntero a la primera posicion del vector para almacenar cada uno de los 
*                          valores de la suma de prefijos.
* @param unsigned long size: tamaño de cada uno de los vectores.
* @return:
*/
void computeparallelprefix(int *iplist, int *_pprefixsum, unsigned long size)
{
  int nthr, *z, *x = _pprefixsum;
  
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