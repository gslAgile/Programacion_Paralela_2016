#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "omp.h"

/* Definicion de Macros*/
#define N 100000000 //33554432 // 2 ^ 25
#define porcion N/4 /* Porcion de iteraciones*/


/* Declaracion de funciones*/
void computeparallelprefix(int *iplist, int *_pprefixsum, unsigned long size);
void computeparallelprefix2(int *iplist, int *_pprefixsum, unsigned long size, int *pz, int *pw);
void computeparallelprefix3(int *iplist, int *_pprefixsum, unsigned long size, int *pz, int *pw);
void for_par(int *py, int *pw, int *iplist, unsigned long psize);
void for_impar(int *py, int *pw, unsigned long psize);

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
  int *iplist, *pprefixsum, *z, *w;
  double t_init, t_final, t_procedural, t_paralelo, speedUp, porcentaje;
  int i, maxThrs=omp_get_max_threads(), nthreads=omp_get_num_threads();
  char sim=37; // simbolo de caracter equvalente a %

/* Inicializacion de valores*/
  omp_set_num_threads(nthreads);
  iplist = (int*) malloc(sizeof(int) * N);
  pprefixsum = (int*) malloc(sizeof(int) * N);
  z = (int*) malloc(sizeof(int) * (N/2));
  w = (int*) malloc(sizeof(int) * (N/2));

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
  //computeparallelprefix(iplist, pprefixsum, N);
  //computeparallelprefix2(iplist, pprefixsum, N, z, w);
  computeparallelprefix3(iplist, pprefixsum, N, z, w);

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
  printf("SpeedUp practico: %0.4lf\n", speedUp);
  porcentaje = (speedUp*100)-100;
  printf("Velocidad paralela: %.2lf mas rapido aproximadamente\n", speedUp);
  printf("Porcentaje de mejora: %3.0lf%c aproximadamente\n\n", porcentaje, sim);

  printf("Ultimo dato obtenido: %d \n\n", pprefixsum[N-1]);
  printf("%d\n", pprefixsum[0]);
  printf("%d\n", pprefixsum[N-1]);
  printf("%d\n", pprefixsum[N-2]);
  printf("%d\n", pprefixsum[N-3]);
  printf("%d\n", pprefixsum[N-4]);
  //for(i=0; i<N; i++) printf("%d ", (i+1)*(i+2)/2); printf("\n");

  free(iplist);
  free(pprefixsum);
  free(z);
  free(w);
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

/**/
void computeparallelprefix2(int *iplist, int *_pprefixsum, unsigned long size, int *pz, int *pw)
{
  int i, j=0, k=0, l=0, *z=pz, *y = _pprefixsum, *w=pw, sum = 0;
  
  #pragma omp parallel private(i) firstprivate(j,k,l,sum)
  {
    /* Calculo de suma de pares*/
    #pragma omp for schedule(static) 
    for(i=0; i<size/2; i=i+2) {
      z[j]= iplist[i]+ iplist[i+1];
      j++;
    }
    /* Calculo recursivo de los prefijos sum w*/
    #pragma omp for schedule(static) 
    for(i=0; i<size/2; i=i+2) {
      sum += z[i];
      w[i] = sum;
    }

    #pragma omp single
    {
      y[0] = iplist[0];
      y[1] = z[0];
      y[2] = z[0] + iplist[2];
    }

    #pragma omp barrier
    #pragma omp for schedule(static)
      for(i=4; i<size; i=i+2) {
        y[i]= w[k]+iplist[i]; k++;
      }

    #pragma omp for schedule(static)
      for(i=3; i<size; i=i+2) {
        y[i]= w[l]; l++;
      }
  }
}

/**/
void computeparallelprefix3(int *iplist, int *_pprefixsum, unsigned long size, int *pz, int *pw)
{
  int i, j=0, *z=pz, *y = _pprefixsum, *w=pw, sum = 0;
  
  #pragma omp parallel private(i) firstprivate(j,sum)
  {
    /* Calculo de suma de pares*/
    #pragma omp for schedule(static) 
    for(i=0; i<size/2; i=i+2) {
      z[j]= iplist[i]+ iplist[i+1];
      j++;
    }
    /* Calculo recursivo de los prefijos sum w*/
    #pragma omp for schedule(static) 
    for(i=0; i<size/2; i=i+2) {
      sum += z[i];
      w[i] = sum;
    }

    #pragma omp single
    {
      y[0] = iplist[0];
      y[1] = z[0];
      y[2] = z[0] + iplist[2];
    }

    #pragma omp barrier
    #pragma omp sections
    {
        #pragma omp section
        {
            for_impar(y,w,size);
        }

        #pragma omp section
        {
            for_par(y,w,iplist,size);
        }
    }
  }
}

/**/
void for_par(int *py, int *pw, int *iplist, unsigned long psize)
{
  int i, j=0;
  /**/
  #pragma omp parallel for private(i) firstprivate(j) schedule(static)
  for(i=4; i<psize; i=i+2) {
    py[i]= pw[j]+iplist[i]; j++;
  }
}

/**/
void for_impar(int *py, int *pw, unsigned long psize)
{
  int i, j=0;
  /**/
  #pragma omp arallel for private(i) firstprivate(j) schedule(static)
  for(i=3; i<psize; i=i+2) {
    py[i]= pw[j]; j++;
  }
}