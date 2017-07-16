/*
*   b) Resuelve el problema con #pragma omp atomic.
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
 
    #pragma omp parallel for reduction( +:sum )
    for (i = 1; i <= n; i++) {
        double x = h * ((double)i - 0.5);
        sum += (4.0 / (1.0 + x*x));
    }
 
    double pi = sum * h;
    printf("El valor aproximado de PI es: %.8lf , con un error de %.8lf\n\n", pi, fabs(pi -PI25DT));
    return 0;
}

/*
*  Otra solución mas eficiente que las anteriores seria el uso de la 
clausula reduction. Cuando la variable compartida puede ser tratada por 
separado y luego mezclar los resultado de cada hebra la aplicación de esta
clausula es lo mas eficiente.
recution crea una copia local a cada hebra, cuando terminan todas las hebras
terminan las iteraciones se aplica la operación de reducción en nuestro 
caso se suman todas.
*/