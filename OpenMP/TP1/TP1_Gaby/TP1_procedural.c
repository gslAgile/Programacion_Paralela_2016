#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define TAM 100000000

/* Declaracion de variables*/
int contenedor[TAM];
int i, j, valor, resultado;
double tiempoFinal = 0, t_ini, t_fin;

/* Funcion principal*/
void main(void) {

/* Inicializacion e valores*/
resultado = 0;

for(i = 0; i<TAM; i++) {
	contenedor[i] = 1;
	//print("%d\n", contenedor[i]);
}

/* Comienzo de suma de prefijo paralela*/

t_ini = omp_get_wtime(); // inicio de cuenta de tiempo

for(i = 0; i<TAM; i++) {
	resultado = resultado + contenedor[i];
	contenedor[i] = resultado; // +=
	//printf("%d\n", contenedor[i]);
}

t_fin = t_ini = omp_get_wtime(); // fin de cuenta de tiempo

tiempoFinal = (t_fin - t_ini);

printf(" Tiempo de total: %.2f segundos\n\n", tiempoFinal);

printf("%d\n", contenedor[999995]);
printf("%d\n", contenedor[999996]);
printf("%d\n", contenedor[999997]);
printf("%d\n", contenedor[999998]);
printf("%d\n", contenedor[999999]);

}