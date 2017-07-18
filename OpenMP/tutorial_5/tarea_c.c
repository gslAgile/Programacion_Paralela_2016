/*
*   c) Añade la clausula if con la condición que creas conveniente.
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
 
int main ()
{
    unsigned int N;
    printf("introduce el tamaño del vector: ");
    scanf( "%i", &N);
    printf("\n");
    double timeIni, timeFin;
    int x[N], y[N], i;

    /* Inicializacion de vectores*/
    for(i=0; i<N; i++)
        x[i] = y[i]=0;

    timeIni = omp_get_wtime();
        
    //Paralelo
    //Escriba el mismo algoritmo de forma paralela
    #pragma omp parallel private(i) if(N>100000)
    {
        # pragma omp single
        printf("Numero de hilos utilizados : %i \n",omp_get_num_threads());

        #pragma omp for 
        for(i=1; i<N; i++)
        {
            x[i] = y[i-1] * 2;
            y[i] = y[i] + i;
        }
    }
 
    timeFin = omp_get_wtime();
    printf("Tiempo tardado paralelo = %.4lf milisegundos\n\n", ((timeFin - timeIni)*1000));

    return 0;
}