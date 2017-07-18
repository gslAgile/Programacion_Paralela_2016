/*
*   a) Usa #pragma omp sections para ejecutar las dos tareas a la vez. Muestra que hebra ejecuta cada section.
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
/* Funciones*/
void tarea_uno(){
    sleep(2);
}
 
void tarea_dos(){
    sleep(4);
}
 
/* Funcion Principal*/
int main (){
 
    double timeIni, timeFin;
    int nthread;

    omp_set_num_threads(2);
 
    timeIni = omp_get_wtime();

    #pragma omp parallel private(nthread)
    {
        nthread = omp_get_thread_num();
        #pragma omp sections
        {
            #pragma omp section
            {
                printf("Hilo %i ejecutando tarea 1 \n", nthread);
                tarea_uno();
            }

            #pragma omp section
            {
                printf("Hilo %i ejecutando tarea 2\n\n", nthread);
                tarea_dos();
            }
        }

    }
 
    timeFin = omp_get_wtime();

    printf("Tiempo tardado = %.4lf segundos\n\n", (timeFin - timeIni));

    return 0;
}