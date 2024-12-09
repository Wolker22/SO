/*Implemente un programa que cree un número N de hilos, donde N será un argumento al programa.
Tenga en cuenta los siguientes supuestos y condiciones:
 Los N hilos comparten dos variables, una llamada "par" e "impar" inicializadas a 0.
 Los hilos deben recibir un valor entero de 1 a N que indica el orden en el cuál se ha creado
el hilo. Así, el primer hilo creado recibe 1, el segundo 2, etc.
 Cada hilo debe generar 5 números aleatorios entre 0 y 10.
 Los hilos de índice de creación par deben ir sumando a la variable "par" los números que
generan.
 Los hilos de índice de creación impar deben ir sumando a la variable "impar" los números
que generan.
 Una vez finalizado cada hilo, éste debe devolver al programa principal la suma total de los
números que ha generado.
 El programa principal debe mostrar la suma devuelta por cada hebra y el valor final de las
variables “par” e “impar”.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct mystruct
{
    int par;
    int impar;
    int i;
    pthread_mutex_t *mutex;
};

void *mifuncion(void *p)
{
    struct mystruct *jar = (struct mystruct *)p;
    for (int i = 0; i < 5; i++)
    {
        int n = rand() % (11) + 10;
        if (pthread_mutex_lock(jar->mutex) != 0)
        {
            perror("Error al bloquear hilo\n");
            exit(EXIT_FAILURE);
        }
        if (n % 2 == 0)
        {
            jar->par = jar->par + 1;
        }
        if (n % 2 != 0)
        {
            jar->impar = jar->impar + 1;
        }
        if (pthread_mutex_unlock(jar->mutex) != 0)
        {
            perror("Error al bloquear hilo\n");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("Error al iniciar el programa\n");
        exit(EXIT_FAILURE);
    }
    int n = atoi(argv[1]);
    srand(time(NULL));

    struct mystruct *p = malloc(sizeof(struct mystruct)*n);
    pthread_t h[n];
    pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

    for (int i = 0; i < n; i++)
    {
        p[i].i = i;
        p[i].impar = 0;
        p[i].par = 0;
        p[i].mutex = &m;
        if (pthread_create(&h[i], NULL, (void *)mifuncion, (void *)&p[i]) != 0)
        {
            perror("Error al iniciar el crear el hilo\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (pthread_join(h[i], NULL) != 0)
        {
            perror("Error al recoger el hilo\n");
            exit(EXIT_FAILURE);
        }
        printf("PAR: %d, IMPAR: %d, Del hilo numero: %d\n", p[i].par, p[i].impar, p[i].i);
    }
}