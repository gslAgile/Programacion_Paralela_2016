#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/* Definicion de Macros*/
#define TAM 100000000 /* Tamaño maximo de vectores*/
#define porcion 100000 /* Porcion de iteraciones*/

/* Declaracion de funciones*/
void inicializar_vector(int * vector);


/* Funcion principal*/
int main(void) {

	/* Declaracion de variables*/
	int contenedor[TAM];
	int i, j, valor, resultado;
	double tiempoProcedural = 0, tiempoCarga = 0, t_ini, t_ini0, t_fin, SpeedUps;

	/* Inicializacion e valores*/
	resultado = 0;

	t_ini0 = omp_get_wtime(); // inicio de tiempo total

	inicializar_vector(contenedor);

	/* Comienzo de suma de prefijo paralela*/

	t_ini = omp_get_wtime(); // inicio de cuenta de tiempo

	for(i = 0; i<TAM; i++) {
		resultado = resultado + contenedor[i];
		contenedor[i] = resultado; // +=
		//printf("%d\n", contenedor[i]);
	}

	t_fin = omp_get_wtime(); // fin de cuenta de tiempo

	/* Calculos finales*/
	tiempoCarga = (t_ini - t_ini0);
	tiempoProcedural = (t_fin - t_ini);
	SpeedUps = tiempoProcedural/tiempoProcedural;


	/* Presentacion de datos*/
	printf("Caracteristicas del sistema\n");
	printf("Numero de hilos disponibles: \n");
	printf("Hilos usados para la inicializacion de vector: \n");
	printf("Hilos usados para Calculos: \n");

	printf(" Tiempo de carga de vector procedural: %.2f segundos\n", tiempoCarga);
	printf(" Tiempo de total de calculo procedural: %.2f segundos\n", tiempoProcedural);
	printf(" Speedups procedural: %.2f \n\n", SpeedUps);

	printf("%s\n", "Ultimos valores obtenidos: ");
	printf("%d\n", contenedor[999995]);
	printf("%d\n", contenedor[999996]);
	printf("%d\n", contenedor[999997]);
	printf("%d\n", contenedor[999998]);
	printf("%d\n", contenedor[999999]);

	return 0;

}

/* Implementacion de funciones*/


/*
* Descripcion:
* @param *vector: puntero del tipo int que indica la posicion de un vector
* @return: no aplica
*/
void inicializar_vector(int * vector)
{
	int i;
	int nthreads_max= omp_get_max_threads();
	omp_set_num_threads(nthreads_max);

	#pragma opm parallel for private(i) schedule(auto)
	{
		for(i = 0; i<TAM; i++)
			vector[i] = 1;
	}
}