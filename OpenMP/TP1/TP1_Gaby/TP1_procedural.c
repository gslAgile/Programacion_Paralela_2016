#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM 100000000

int contenedor[TAM];
int i, j, valor, resultado;
double fin = 0;
clock_t t_ini, t_fin;

void main(void) {

resultado = 0;

for(i = 0; i<TAM; i++) {
	contenedor[i] = 1;
	//print("%d\n", contenedor[i]);
}

t_ini = clock();

for(i = 0; i<TAM; i++) {
	resultado = resultado + contenedor[i];
	contenedor[i] = resultado; // +=
	//printf("%d\n", contenedor[i]);
}

t_fin = clock();
fin = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
printf("%d\n", contenedor[999995]);
printf("%d\n", contenedor[999996]);
printf("%d\n", contenedor[999997]);
printf("%d\n", contenedor[999998]);
printf("%d\n", contenedor[999999]);

printf("%f segundos\n", fin);

}