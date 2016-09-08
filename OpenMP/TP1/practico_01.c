#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM 100000000

int contenedor[TAM];
int i, j, valor;
double fin = 0;
clock_t t_ini, t_fin;

void main(void) {

t_ini = clock();

for(i = 0; i<TAM; i++) {
	valor = rand() % 20;
	contenedor[i] = valor;
	
}

for(i = 0; i<TAM; i++) {
	contenedor[i] = contenedor[i] + contenedor[i-1];
	//printf("%d\n", contenedor[i]);
}

t_fin = clock();
fin = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
printf("%f segundos\n", fin);

}

