/*
*   b) Establece una marca de tiempo en el momento que salen del bloque de ejecución sections.
* Si una tarea tarda mas que la otra, las hebras registraran tiempo con diferencia aproximadamente 
* igual a la diferencia de tiempo de las tareas. ¿Esto es cierto? En caso contrario ¿Que hay que
* hacer para que esto ocurra?.
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
 
    double timeIni, timeFin, time_s1, time_s2;
    int nthread;

    omp_set_num_threads(2);
 
    timeIni = omp_get_wtime();

    #pragma omp parallel private(nthread)
    {
        nthread = omp_get_thread_num();
        #pragma omp sections //nowait /* con nowait se librea el hilo de la seccion sin esperar a los demas.*/
        {
            #pragma omp section
            {
                printf("Hilo %i ejecutando tarea 1 \n", nthread);
                tarea_uno();
                //time_s1 = omp_get_wtime(); /*otra opcion valida para capturr los tiempo de salida de la seccion 1 de manera correcta*/
            }

            #pragma omp section
            {
                printf("Hilo %i ejecutando tarea 2\n\n", nthread);
                tarea_dos();
                //time_s2x = omp_get_wtime(); /*otra opcion valida para capturr los tiempo de salida de la seccion 2 de manera correcta*/
            }
        }
        printf("Tiempo de salida hilo %i = %.4lf segundos.\n\n", nthread, (omp_get_wtime() - timeIni));
    }
 
    timeFin = omp_get_wtime();

    /* Usando time_s1 y time_s2 */
    //printf("Tiempo salida hilo de tarea 1 = %.4lf segundos\n\n", (time_s1 - timeIni));
    //printf("Tiempo salida hilo de tarea 2 = %.4lf segundos\n\n", (time_s2 - timeIni));

    /* Tiempo total*/
    printf("Tiempo tardado = %.4lf segundos\n\n", (timeFin - timeIni));

    return 0;
}

/* Conclusion: los hilos se sincronizan cuando salen de la region paralela. Para evitar la sincronizacion  de hilos que hacen uso
*  de secciones se puede utilizar la sentencia no wait.
*/