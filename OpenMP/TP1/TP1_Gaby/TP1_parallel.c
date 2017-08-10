#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "omp.h"

/* Definicion de Macros*/
#define N 100000000 //33554432 // 2 ^ 25
#define porcion N/4 /* Porcion de iteraciones*/
#define size_zw N/2 /* Porcion de iteraciones*/
#define tam_v 100 /* tamaño fijo para vectores de estadiscas*/


/* Declaracion de funciones*/
void iniciar_vectores_est(double *tp_est1, double *tp_est2, double *tp_est3, int *im1, int *im2,int *im3);
void ver_estadisticas(double *tp_est1, double *tp_est2, double *tp_est3, int *im1, int *im2, int *im3, double speedup[][tam_v], double porcentajes[][tam_v]);
int convertir_cadena(char *pc);
void resultados_obtenidos(double par_t_paralelo, double par_t_procedural, int p_ntrh);
void datos_arreglo(int *pprefixsum);
void computeparallelprefix(int *iplist, int *_pprefixsum, unsigned long size);
void computeparallelprefix2(int *iplist, int *_pprefixsum, int *pz, int *pw);
void computeparallelprefix3(int *iplist, int *_pprefixsum, int *pz, int *pw);
void for_par(int *py, int *pw, int *iplist);
void for_impar(int *py, int *pw);

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
  int *iplist, *pprefixsum, *z, *w, opcion, im1[tam_v], im2[tam_v], im3[tam_v]; /* im: iterador de metodo*/
  double t_init, t_final, t_procedural, t_paralelo, speedUp[3][tam_v], porcentajes[3][tam_v];
  double ts_est1[tam_v], ts_est2[tam_v], ts_est3[tam_v], tp_est1[tam_v], tp_est2[tam_v], tp_est3[tam_v]; /* tiempos estadisticos en serie y paralelo para cada metodo*/
  int i, maxThrs=omp_get_max_threads(), nthreads=omp_get_num_threads();
  char sim=37; // simbolo de caracter equvalente a %
  char cadena[256];

/* Inicializacion de valores*/
  omp_set_num_threads(nthreads);
  iplist = (int*) malloc(sizeof(int) * N);
  pprefixsum = (int*) malloc(sizeof(int) * N);
  z = (int*) malloc(sizeof(int) * (N/2));
  w = (int*) malloc(sizeof(int) * (N/2));
  iniciar_vectores_est(tp_est1, tp_est2, tp_est3, im1, im2, im3); /* iniciar vectores estadisticos*/

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
      printf( "\n   0. Cambiar numero de hilos para calculos paralelos.");
      printf( "\n   1. Resolver Algoritmo de Sum Prefix Parallel con OPCION 1.");
      printf( "\n   2. Resolver Algoritmo de Sum Prefix Parallel con OPCION 2.");
      printf( "\n   3. Resolver Algoritmo de Sum Prefix Parallel con OPCION 3.");
      printf( "\n   4. Ver estadisticas de algoritmos.");
      printf( "\n   5. Salir." );
      printf( "\n\n   > Su seleccion de opcion (0-5): ");

      scanf( "%d", &opcion );

      /* Inicio del anidamiento */

      switch ( opcion )
      {
        case 0: printf( "\n   > Introduzca el numero de hilos deseado: ");
                scanf( "%s", cadena);
                nthreads= convertir_cadena(cadena);
                omp_set_num_threads(nthreads);
                break;

        case 1: /*Calculo parallelo*/
                if(nthreads>1)
                {
                  im1[nthreads]++;
                  omp_set_num_threads(nthreads);
                  t_init=omp_get_wtime();
                  computeparallelprefix(iplist, pprefixsum, N);
                  t_final=omp_get_wtime();
                  t_paralelo=(t_final-t_init);
                  tp_est1[nthreads]= tp_est1[nthreads]+t_paralelo; /* acumulacion de tiempos paralelos con metodo 1*/
                  datos_arreglo(pprefixsum);
                }
                /*Calculo procedural*/
                im1[1]++;
                omp_set_num_threads(1);
                t_init=omp_get_wtime();
                computeparallelprefix(iplist, pprefixsum, N);
                t_final=omp_get_wtime();
                t_procedural=(t_final-t_init);
                tp_est1[1] = tp_est1[1]+t_procedural; /* acumulacion de tiempos seriales con metodo 1*/
                datos_arreglo(pprefixsum);
                resultados_obtenidos(t_paralelo, t_procedural, nthreads);
                break;

        case 2: /*Calculo parallelo*/
                if(nthreads>1)
                {
                  im2[nthreads]++;
                  omp_set_num_threads(nthreads);
                  t_init=omp_get_wtime();
                  computeparallelprefix2(iplist, pprefixsum, z, w);
                  t_final=omp_get_wtime();
                  t_paralelo=(t_final-t_init);
                  tp_est2[nthreads]= tp_est2[nthreads]+t_paralelo; /* acumulacion de tiempos paralelos con metodo 2*/
                  datos_arreglo(pprefixsum);
                }
                /*Calculo procedural*/
                im2[1]++;
                omp_set_num_threads(1);
                t_init=omp_get_wtime();
                computeparallelprefix2(iplist, pprefixsum, z, w);
                t_final=omp_get_wtime();
                t_procedural=(t_final-t_init);
                tp_est2[1] = tp_est2[1]+t_procedural; /* acumulacion de tiempos seriales con metodo 2*/
                datos_arreglo(pprefixsum);
                resultados_obtenidos(t_paralelo, t_procedural, nthreads);
                break;

        case 3: /*Calculo parallelo*/
                if(nthreads>1)
                {
                  im3[nthreads]++;
                  omp_set_num_threads(nthreads);
                  t_init=omp_get_wtime();
                  computeparallelprefix3(iplist, pprefixsum, z, w);
                  t_final=omp_get_wtime();
                  t_paralelo=(t_final-t_init);
                  tp_est3[nthreads]= tp_est3[nthreads]+t_paralelo; /* acumulacion de tiempos paralelos con metodo 3*/
                  datos_arreglo(pprefixsum);
                }
                /*Calculo procedural*/
                im3[1]++;
                omp_set_num_threads(1);
                t_init=omp_get_wtime();
                computeparallelprefix3(iplist, pprefixsum, z, w);
                t_final=omp_get_wtime();
                t_procedural=(t_final-t_init);
                tp_est3[1] = tp_est3[1]+t_procedural; /* acumulacion de tiempos seriales con metodo 3*/
                datos_arreglo(pprefixsum);
                resultados_obtenidos(t_paralelo, t_procedural, nthreads);
                break;

        case 4: ver_estadisticas(tp_est1, tp_est2, tp_est3, im1, im2, im3, speedUp, porcentajes);
                break;

        case 5: printf( "\n   > Saliendo de aplicacion.\n\n");
                break;

        default: printf("\n   > Opcion no valida. Intente nuevamente segun opciones del menu.\n " );
       }
       /* Fin del anidamiento */
  }

  sleep(0.5);

  free(iplist);
  free(pprefixsum);
  free(z);
  free(w);

  return 0;
}

/* Implementacion de funciones*/

/*
* Descripcion:
* @param 
*/
void iniciar_vectores_est(double *tp_est1, double *tp_est2, double *tp_est3, int *im1, int *im2,int *im3)
{
  int i;
  for(i=0; i<tam_v; i++)
    tp_est1[i]=tp_est2[i]=tp_est3[i]=im1[i]=im2[i]=im3[i]=0;
}

/*
* Descripcion:
* @param .
*/
void ver_estadisticas(double *tp_est1, double *tp_est2, double *tp_est3, int *im1, int *im2, int *im3, double speedup[][tam_v], double porcentajes[][tam_v])
{
  int i, j =0, fa1, fa2, fa3; /* fa: flag algoritmo 1,2,3*/;
  char sim=37; // simbolo de caracter equvalente a %
  fa1=fa2=fa3=0;

  printf("\n-----------------------------------------------------------------------------");
  printf( "\n   >>>_              ESTADISTICAS DE ALGORITMOS               _<<<\n");
  printf("-----------------------------------------------------------------------------\n");

  /* METODO 1*/
  for(i=0; i<tam_v-1; i++)
  {
    if(tp_est1[i+1]>0 && im1[i+1]>0)
    {
      if(j==0)
      {
        printf("\n-----------------------------------------------------------------------------");
        printf( "\n   >>>_                      ALGORITMO 1                      _<<<\n");
        printf("-----------------------------------------------------------------------------\n\n");
        printf("      |  Cores  |  Tiempo  | SpeedUp  |  Porcentaje  |  Muestras  |\n");
        j++; fa1++;
      }
      /* Calculo de speedUp practico*/
      speedup[0][i+1] = (tp_est1[1]/im1[1])/(tp_est1[i+1]/im1[i+1]);
      porcentajes[0][i+1] = (speedup[0][i+1]*100)-100;
      printf("      -------------------------------------------------------------\n");
      printf("      |  %*.d    |  %0.4lf  |  %0.4lf  |  %5.0lf%c      |  %5.d     |\n",3, i+1, (tp_est1[i+1]/im1[i+1]), speedup[0][i+1], porcentajes[0][i+1], sim, im1[i+1]);
    }
  }

  /* METODO 2*/
  j=0;
  for(i=0; i<tam_v-1; i++)
  {
    if(tp_est2[i+1]>0 && im2[i+1]>0)
    {
      if(j==0)
      {
        printf("\n-----------------------------------------------------------------------------");
        printf( "\n   >>>_                     ALGORITMO 2                       _<<<\n");
        printf("-----------------------------------------------------------------------------\n\n");
        printf("      |  Cores  |  Tiempo  | SpeedUp  |  Porcentaje  |  Muestras  |\n");
        j++; fa2++;
      }
      /* Calculo de speedUp practico*/
      speedup[1][i+1] = (tp_est2[1]/im2[1])/(tp_est2[i+1]/im2[i+1]);
      porcentajes[1][i+1] = (speedup[1][i+1]*100)-100;
      printf("      -------------------------------------------------------------\n");
      printf("      |  %*.d    |  %0.4lf  |  %0.4lf  |  %5.0lf%c      |  %5.d     |\n",3, i+1, (tp_est2[i+1]/im2[i+1]), speedup[1][i+1], porcentajes[1][i+1], sim, im2[i+1]);
    }
  }

  /* METODO 3*/
  j=0;
  for(i=0; i<tam_v-1; i++)
  {
    if(tp_est3[i+1]>0 && im3[i+1]>0)
    {
      if(j==0)
      {
        printf("\n-----------------------------------------------------------------------------");
        printf( "\n   >>>_                      ALGORITMO 3                      _<<<\n");
        printf("-----------------------------------------------------------------------------\n\n");
        printf("      |  Cores  |  Tiempo  | SpeedUp  |  Porcentaje  |  Muestras  |\n");
        j++; fa3++;
      }
      /* Calculo de speedUp practico*/
      speedup[2][i+1] = (tp_est3[1]/im3[1])/(tp_est3[i+1]/im3[i+1]);
      porcentajes[2][i+1] = (speedup[2][i+1]*100)-100;
      printf("      -------------------------------------------------------------\n");
      printf("      |  %*.d    |  %0.4lf  |  %0.4lf  |  %5.0lf%c      |  %5.d     |\n",3, i+1, (tp_est3[i+1]/im3[i+1]), speedup[2][i+1], porcentajes[2][i+1], sim, im3[i+1]);
    }
  }

  if(fa1==0){
    printf("\n-----------------------------------------------------------------------------");
    printf("\n   > Sin estadisticas para el algoritmo 1.");}

  if(fa2==0){
    printf("\n-----------------------------------------------------------------------------");
    printf("\n   > Sin estadisticas para el algoritmo 2.");
  }

  if(fa3==0){
    printf("\n-----------------------------------------------------------------------------");
    printf("\n   > Sin estadisticas para el algoritmo 3.");
  }
  printf("\n-----------------------------------------------------------------------------\n\n");
  /*speedup[0][0]= 18; //prueba*/
}

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
void resultados_obtenidos(double par_t_paralelo, double par_t_procedural, int p_nthr)
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
}

/*
* Descripcion:
* @param :
*/
void datos_arreglo(int *pprefixsum)
{
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
  int i, nthr, *z=pz, *y = _pprefixsum, *w=pw, *aux;
  
  #pragma omp parallel private(i)
  {
    /* Calculo de suma de pares*/
    #pragma omp for schedule(static) 
    for(i=0; i<size_zw; i++){
      z[i]= iplist[i*2]+ iplist[(i*2)+1];
    }

    #pragma omp single
    {
      nthr = omp_get_num_threads();
      aux = malloc(sizeof(int)*nthr+1);/* aux: contenedor de cada uno de las sumatorias paralelas (de pares) por cada hilo*/
      aux[0]=0; /* primer valor en cero*/

      /* Calculo de primer valor de sumatorias de pares z*/
      w[0]=z[0]+z[1];

      /* Calculos de primeros valores de la sum prefix segun primeros datos obtenidos*/
      y[0] = iplist[0];
      y[1] = z[0];
      y[2] = z[0] + iplist[2];
    }

    /* Calculo recursivo de los prefijos sum w*/
    int tid = omp_get_thread_num(); /* identificacion de cada hilo*/
    int sum = z[0]+z[1]; /* inicializacion de primer valor de la sumatoria recursiva*/
    #pragma omp for schedule(static) 
    for(i=1; i<size_zw; i++) {
      sum +=z[i+1];
      w[i] = sum;
    }
    aux[tid+1] = sum; /* almacenamiento de sumatoria segun calculo de cada hilo*/
    #pragma omp barrier /* punto de sincronizacion */

    int offset = 0; /* Calculo e los desplazamientos segun cada caso*/
    for(i=1; i<(tid+1); i++) {
        offset += aux[i]-z[0]-z[1]; /* se descuentan los z[0] y z[1] que solo corresponden a primer sumatoria*/
    }

    /* Calculo de valores impares de la sum prefix*/
    #pragma omp for schedule(static)
      for(i=0; i<size_zw; i++) {
        y[(2*i)+3]= w[i]+offset;
      }

    /* Calculo de valores pares de la sum prefix*/
    #pragma omp for schedule(static)
      for(i=0; i<size_zw; i++) {
        y[(2*i)+4]= w[i]+iplist[(2*i)+4]+offset; 
      }
  }
}

/**/
void computeparallelprefix3(int *iplist, int *_pprefixsum, int *pz, int *pw)
{
  int i, *z=pz, *y = _pprefixsum, *w=pw;
  
  #pragma omp parallel private(i)
  {
    /* Calculo de suma de pares*/
    #pragma omp for schedule(static) 
    for(i=0; i<size_zw; i++){
      z[i]= iplist[i*2]+ iplist[(i*2)+1];
    }
    #pragma omp single
    {
      /* Calculos de primeros valores de la sum prefix segun primeros datos obtenidos*/
      y[0] = iplist[0];
      y[1] = z[0];
      y[2] = z[0] + iplist[2];

      /* Calculo de primer valor de sumatorias de pares z*/
      w[0]=z[0]+z[1];
      
      /* Calculo recursivo de los prefijos sum w*/
      for(i=1; i<(size_zw-1); i++)
      w[i]=w[i-1]+z[i+1];
    }

    #pragma omp barrier /* punto de sincronizacion*/

    /* Calculo de valores pares e impares de la sum prefix de manera paralela*/
    #pragma omp sections
    {
        /* Calculo de valores impares de la sum prefix*/
        #pragma omp section
        {
            for_impar(y,w);
        }
        /* Calculo de valores pares de la sum prefix*/
        #pragma omp section
        {
            for_par(y,w,iplist);
        }
    }
  }
}

/**/
void for_par(int *py, int *pw, int *iplist)
{
  int i;
  /**/
  #pragma omp parallel for private(i) schedule(static)
  for(i=0; i<size_zw; i++) {
        py[(2*i)+4]= pw[i]+iplist[(2*i)+4]; 
      }
  // for(i=4; i<psize; i=i+2) {
  //   py[i]= pw[j]+iplist[i]; j++;
  // }
}

/**/
void for_impar(int *py, int *pw)
{
  int i;
  /**/
  #pragma omp arallel for private(i) schedule(static)
  for(i=0; i<size_zw; i++) {
        py[(2*i)+3]= pw[i];
      }
  // for(i=3; i<psize; i=i+2) {
  //   py[i]= pw[j]; j++;
  // }
}