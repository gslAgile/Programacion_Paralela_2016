# Programacion Paralela 2016-2017

## [Ingenieria en Computacion - FCEFyN - UNC](http://computacion.efn.uncor.edu)

  **Profesor de curso:** Ing. Gustavo Wolfmann
  
  **Integrantes:** Garcia Cannata Nicolas - Colazo Danilo - Gomez Palacio Nicolas - Sosa Ludueña Gabriel
  
  **Tema:** Desarrollo programacion parlela en el SO Linux
  

**TPFinal: Trabajo Practico Final 2016-2017**

Resolver en paralelo el algoritmo de Canny para la detección de contornos de objetos en imágenes.
Pueden ver como trabaja el algoritmo en:

[enlace externo 1](http://homepages.inf.ed.ac.uk/rbf/HIPR2/canny.htm)
[enlace externo 2](http://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_imgproc/py_canny/py_canny.html)

y un video en ingles en:
[video 1](https://www.youtube.com/watch?v=sRFM5IEqR2w)

el código secuencial sobre el cual tienen que trabajar esta en el cluster, en la carpeta pp16, y es el archivo canny.c
hay que compilarlo con gcc incluyendo la libreria matemática con la directiva de compilación -lm

> gcc -lm -o canny canny.c

trabajar con una imagen relativamente grande, donde pueda apreciarse la ventaja del algoritmo paralelo.
el programa trabaja con imágenes tipo .pgm . pueden tomar cualquier imagen y exportarla en ese formato usando gimp.
para paralelizar, usar openmp, cuda y mpi. 
detectar primero cuales son los puntos del programa que insumen mas cómputo cuya paralelización sea justificada

