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
 
    #pragma omp parallel for shared( sum )
    for (i = 1; i <= n; i++) {
        double x = h * ((double)i - 0.5);
        #pragma omp atomic
        sum += (4.0 / (1.0 + x*x));
    }
 
    double pi = sum * h;
    printf("El valor aproximado de PI es: %.8lf , con un error de %.8lf\n\n", pi, fabs(pi -PI25DT));
    return 0;
}

/*
*  La directiva #pragma omp critical se queda grande ya que esta permite 
la exclusión mutua de bloques de código. Para nuestro caso al tratarse de
 una instrucción simple podemos decir que sea una instrucción atómica, 
 siento esto mas eficiente que una región critica.
*/