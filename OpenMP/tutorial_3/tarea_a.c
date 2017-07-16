/*
*   a) Resuelve el problema con #pragma omp critical.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
 
int main(int argc, char *argv[])
{
    int n,i;
    printf("Introduce la precision del calculo (n > 0): ");
    scanf( "%i", &n);
    double PI25DT = 3.141592653589793238462643;
    double h = 1.0 / (double) n;
    double sum = 0.0;
 
    #pragma omp parallel for shared( sum )
    for (i = 1; i <= n; i++) {
        double x = h * ((double)i - 0.5);
        #pragma omp critical
        sum += (4.0 / (1.0 + x*x));
    }
 
    double pi = sum * h;
    printf("El valor aproximado de PI es: %.8lf , con un error de %.8lf\n\n", pi, fabs(pi -PI25DT));
    return 0;
}

/*
*  Añadiendo la clausula #pragma omp critical antes de la instrucción que
escribe la variable sum conseguimos que ese conjunto de instrucciones no
se ejecute a la vez por varias hebras (exclusión mutua). Esta clausula 
permite un nombre como parámetro que permite coexistir con otras regiones 
criticas, pudiendo entre dos regiones con distinto nombre ejecutarse 
simultaneamente.
*/