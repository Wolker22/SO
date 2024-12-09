#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_THREADS 100 // Número máximo de hilos

struct numeros
{
    int par;
    int impar;
    int i;
    int resultado;
};

struct numeros resultados[MAX_THREADS]; // Arreglo de estructuras para almacenar los resultados de cada hilo

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void *funcion(void *arg)
{
    struct numeros *num = (struct numeros *)arg;

    int s, numero;
    int *res = malloc(sizeof(int));
    *res = 0;

    s = pthread_mutex_lock(&mtx);
    if (s != 0)
    {
        printf("mutex_lock error...\n");
        pthread_exit(NULL);
    }

    for (int i = 0; i < 5; i++)
    {
        numero = rand() % 11;
        *res = *res + numero;
    }

    if ((*res) % 2 == 0)
    {
        num->par = num->par + 1;
    }
    else
    {
        num->impar = num->impar + 1;
    }

    num->resultado = *res; // Guardar el resultado en la estructura

    s = pthread_mutex_unlock(&mtx);
    if (s != 0)
    {
        printf("mutex_unlock error...\n");
        pthread_exit(NULL);
    }

    pthread_exit((void *)res);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("ERROR\n");
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);
    int *res;
    pthread_t v[MAX_THREADS];
    srand(time(NULL));

    for (int i = 0; i < N; i++)
    {
        resultados[i].i = i + 1;
        resultados[i].impar = 0;
        resultados[i].par = 0;

        if (pthread_create(&v[i], NULL, funcion, (void *)&resultados[i]) != 0)
        {
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int j = 0; j < N; j++)
    {
        if (pthread_join(v[j], (void **)&res))
        {
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);
        }
        printf("El valor del hilo %d es %d\n", resultados[j].i, *res);
        free(res); // Liberar memoria asignada para 'res'
    }

    // Mostrar los resultados totales
    int totalImpares = 0, totalPares = 0;
    for (int k = 0; k < N; k++)
    {
        totalImpares += resultados[k].impar;
        totalPares += resultados[k].par;
    }

    printf("El valor total de los impares es: %d\n", totalImpares);
    printf("El valor total de los pares es: %d\n", totalPares);

    return 0;
}