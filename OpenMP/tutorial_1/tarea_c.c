/*
*
* c) Crea una nueva variable e inicializarla fuera de la región paralela y pasa la como privada
* a la zona paralela, ¿El valor de la variable dentro de la región paralela es correcto? prueba usando fistprivate.
*
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
 
int main (int argc, char *argv[]){
 
int nthreads;
int thread;
int nueva_var =10;

if(argc > 1)
{
	nthreads = atoi(argv[1]); // convertimos el numero char en int
}
else
	nthreads = 10;

omp_set_num_threads(nthreads);
 
#pragma omp parallel private(nthreads, thread) firstprivate(nueva_var)//private(nthreads, thread, nueva_var)
  {
 
  thread = omp_get_thread_num(); // funcion de omp.h que retorna el numero id de hilo
 
  nthreads = omp_get_num_threads(); // funcion de omp.h que retorna la cantidad de hilos

  /* Mostramos resultados por stdout*/
  printf("\nHola Mundo soy el hilo =%d de %d que somos en total. La variable nueva es: %d\n", thread, nthreads, nueva_var);
 
  }

  printf("\n");

  return 0;

  /* Conclusion: 
  *  El valor de la variable nueva dentro de la region paralela no es correcto por mas que alla sido
  *  inicializado fuera de la region paralela. Ya que en esta region como cada hilo se lo asigna localmente
  *  etonces no sabe como inicializar y la deja vacia o con valor basura.
  *  Para evitar esto lo que hacemos es lograr que tome el valor de inicializacion fuera de la region parelela 
  *  que se logra con la la sentencia 'firstprivate'.
  */
 
}