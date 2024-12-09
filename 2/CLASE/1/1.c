#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

void *mifuncion()
{
    float N1 = drand48();
    float N2 = drand48();
    float *sum = malloc(sizeof(float));
    printf("N1: %f\n", N1);
    printf("N2: %f\n", N2);
    *sum = (N1) + (N2);
    pthread_exit((void *)sum);
}

int main(int argc, char **argv)
{

    float *canserbero;
    float sum;
    srand48(time(NULL));
    if (argc != 2)
    {
        printf("ERROR\n");
        return (EXIT_FAILURE);
    }
    int N = atoi(argv[1]);
    pthread_t tid[N];
    for (int i = 0; i < N; i++)
    {
        printf("HILO PRINCIPAL: Se va a crear un hilo...\n");
        if (pthread_create(&tid[i], NULL, (void *)mifuncion, NULL))
        {
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }
    for (int j = 0; j < N; j++)
    {
        printf("Hilo principal: Hilo creado, esperando su finalizacion desde el main()...\n");
        if (pthread_join(tid[j], (void **)&canserbero))
        {
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);
        }
        sum = sum + *canserbero;
    }
    printf("suma: %f\n", sum);
    exit(EXIT_SUCCESS);
}