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
 
    timeIni = omp_get_wtime();
 
    printf("Ejecutando tarea 1 \n");
    tarea_uno();
 
    printf("Ejecutando tarea 2\n\n");
    tarea_dos();
 
 
    timeFin = omp_get_wtime();

    printf("Tiempo tardado = %.4lf segundos\n\n", (timeFin - timeIni));

    return 0;
}