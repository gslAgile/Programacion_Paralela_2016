#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "omp.h"

/* Definicion de Macros*/
#define N 100000000 //33554432 // 2 ^ 25
#define porcion N/4 /* Porcion de iteraciones*/
#define size_zw N/2 /* Porcion de iteraciones*/


/* Declaracion de funciones*/
int convertir_cadena(char *pc);
void resultados_obtenidos(double par_t_paralelo, double par_t_procedural, int p_ntrh, int *pprefixsum);
void computeparallelprefix(int *iplist, int *_pprefixsum, unsigned long size);
void computeparallelprefix2(int *iplist, int *_pprefixsum, int *pz, int *pw);
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
  int *iplist, *pprefixsum, *z, *w, opcion;
  double t_init, t_final, t_procedural, t_paralelo, speedUp, porcentaje;
  int i, maxThrs=omp_get_max_threads(), nthreads=omp_get_num_threads();
  char sim=37; // simbolo de caracter equvalente a %
  char cadena[256];

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
    printf( "\n   >>>_      PARAMETROS RECIBIDOS      _<<<\n");
    printf("\n   > Parametro: %s (convertido a entero %i)\n",argv[1], nthreads);
    if(nthreads < 1)
    {
      printf("\n   > Parametro incorrecto. Se esablecen valores por defecto.\n");
      nthreads =1;
    }
    omp_set_num_threads(nthreads);
  }

  /* --- MENU DE OPCIONES --- */
  while ( opcion != 5)
  {
      printf( "\n   >>>_      CARACTERISTICAS DEL SISTEMA      _<<<\n");
      printf("\n   > Numero de hilos disponibles en el equipo: %i",maxThrs);
      printf("\n   > Hilos a utilizar para calculos paralelos: %i\n", nthreads);

      printf( "\n   >>>_      MENU DE OPCIONES      _<<<\n");
      printf( "\n   1. Cambiar numero de hilos para calculos paralelos.");
      printf( "\n   2. Resolver Algoritmo de Sum Prefix Parallel con OPCION 1.");
      printf( "\n   3. Resolver Algoritmo de Sum Prefix Parallel con OPCION 2.");
      printf( "\n   4. Resolver Algoritmo de Sum Prefix Parallel con OPCION 3.");
      printf( "\n   5. Salir." );
      printf( "\n\n   > Su seleccion de opcion (1-5): ");

      scanf( "%d", &opcion );

      /* Inicio del anidamiento */

      switch ( opcion )
      {
        case 1: printf( "\n   > Introduzca el numero de hilos deseado: ");
                scanf( "%s", cadena);
                nthreads= convertir_cadena(cadena);
                omp_set_num_threads(nthreads);
                break;

        case 2: /*Calculo parallelo*/
                t_init=omp_get_wtime();
                computeparallelprefix(iplist, pprefixsum, N);
                t_final=omp_get_wtime();
                t_paralelo=(t_final-t_init);
                /*Calculo procedural*/
                omp_set_num_threads(1);
                t_init=omp_get_wtime();
                computeparallelprefix(iplist, pprefixsum, N);
                t_final=omp_get_wtime();
                t_procedural=(t_final-t_init);
                omp_set_num_threads(nthreads);
                resultados_obtenidos(t_paralelo, t_procedural, nthreads, pprefixsum);
                break;

        case 3: /*Calculo parallelo*/
                t_init=omp_get_wtime();
                computeparallelprefix2(iplist, pprefixsum, z, w);
                t_final=omp_get_wtime();
                t_paralelo=(t_final-t_init);
                /*Calculo procedural*/
                //omp_set_num_threads(1);
                //t_init=omp_get_wtime();
                //computeparallelprefix2(iplist, pprefixsum, z, w);
                //t_final=omp_get_wtime();
                //t_procedural=(t_final-t_init);
                omp_set_num_threads(nthreads);
                resultados_obtenidos(t_paralelo, t_procedural, nthreads, pprefixsum);
                break;

        case 4: /*Calculo parallelo*/
                t_init=omp_get_wtime();
                computeparallelprefix3(iplist, pprefixsum, N, z, w);
                t_final=omp_get_wtime();
                t_paralelo=(t_final-t_init);
                /*Calculo procedural*/
                omp_set_num_threads(1);
                t_init=omp_get_wtime();
                computeparallelprefix3(iplist, pprefixsum, N, z, w);
                t_final=omp_get_wtime();
                t_procedural=(t_final-t_init);
                omp_set_num_threads(nthreads);
                resultados_obtenidos(t_paralelo, t_procedural, nthreads, pprefixsum);
                break;

        case 5: printf( "\n   > Saliendo de aplicacion.\n\n");
                break;

        default: printf("\n   > Opcion no valida. Intente nuevamente segun opciones de menu.\n " );
       }
       /* Fin del anidamiento */
  }

  sleep(1);

  free(iplist);
  free(pprefixsum);
  free(z);
  free(w);

  return 0;
}

/* Implementacion de funciones*/

/*
* Descripcion:
* @param char *pc: puntero a l primera posicion de una cadena.
*/
int convertir_cadena(char *pc)
{
  int num_ent=1; /* Numero entero*/
  num_ent = atoi(pc);
  //printf("Numero entero: %i\n", num_ent);
  if(num_ent<1)
    num_ent=1;

  return num_ent;
}

/*
* Descripcion:
* @param double par_t_paralelo: parametro del tiempo paralelo
*/
void resultados_obtenidos(double par_t_paralelo, double par_t_procedural, int p_nthr, int *pprefixsum)
{
  double speedUp, porcentaje;
  char sim=37; // simbolo de caracter equvalente a %

  printf( "\n   >>>_      RESULTADOS OBTENIDOS      _<<<\n");
  printf("\n   > TIEMPOS");
  printf("\n   > Tiempo paralelo: %0.4lf segundos", par_t_paralelo);
  printf("\n   > Tiempo procedural: %0.4lf segundos\n", par_t_procedural);
  /*Calculo de speedUp (por Ley de Amdahl)*/
  speedUp = (par_t_procedural+par_t_paralelo)/(par_t_procedural+(par_t_paralelo)/p_nthr);
  printf("\n   > SPEEDUP");
  printf("\n   > SpeedUp segun Ley de Amdahl: %0.4lf", speedUp);
  porcentaje = (speedUp*100)-100;
  printf("\n   > Velocidad paralela: %.2lf mas rapido aproximadamente", speedUp);
  printf("\n   > Porcentaje de mejora: %3.0lf%c aproximadamente\n", porcentaje, sim);

  /* Calculo de speedUp practico*/
  speedUp = par_t_procedural/par_t_paralelo;
  printf("\n   > SpeedUp practico: %0.4lf", speedUp);
  porcentaje = (speedUp*100)-100;
  printf("\n   > Velocidad paralela: %.2lf mas rapido aproximadamente", speedUp);
  printf("\n   > Porcentaje de mejora: %3.0lf%c aproximadamente\n", porcentaje, sim);

  printf("\n   > Datos del arreglo:");
  printf("\n   %d", pprefixsum[0]);
  printf("\n   %d", pprefixsum[1]);
  printf("\n   %d", pprefixsum[2]);
  printf("\n   %d", pprefixsum[3]);
  printf("\n   .\n   .\n   .");
  printf("\n   %d", pprefixsum[N-4]);
  printf("\n   %d", pprefixsum[N-3]);
  printf("\n   %d", pprefixsum[N-2]);
  printf("\n   %d\n", pprefixsum[N-1]);
}


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
void computeparallelprefix2(int *iplist, int *_pprefixsum, int *pz, int *pw)
{
  int i, *z=pz, *y = _pprefixsum, *w=pw, sum=0;
  
  #pragma omp parallel private(i)
  {
    /* Calculo de suma de pares*/
    #pragma omp for schedule(static) 
    for(i=0; i<size_zw; i++){
      z[i]= iplist[i*2]+ iplist[(i*2)+1];
    }

    #pragma omp single
    {
      w[0]=z[0]+z[1];
      /* Calculo recursivo de los prefijos sum w*/
      for(i=1; i<(size_zw-1); i++)
      w[i]=w[i-1]+z[i+1];
    }

    /* Calculo recursivo de los prefijos sum w
    #pragma omp for schedule(static)
    for(i=1; i<(size_zw-1); i++) {
      w[i]=w[i-1]+z[i+1];
    }*/

    #pragma omp single
    {
      y[0] = iplist[0];
      y[1] = z[0];
      y[2] = z[0] + iplist[2];
    }
    
    #pragma omp barrier

    #pragma omp for schedule(static)
      for(i=0; i<size_zw; i++) {
        y[(2*i)+3]= w[i];
      }

    //
    #pragma omp for schedule(static)
      for(i=0; i<size_zw; i++) {
        y[(2*i)+4]= w[i]+iplist[(2*i)+4]; 
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
    for(i=0; i<size/2; i++) {
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