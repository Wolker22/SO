#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *camisetas;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *compra(void *N)
{
    int NCamisetas = *(int *)(N);
    int k = rand() % (NCamisetas + 1);
    int NCompra = rand() % (100 - 1 + 1) + 1;

    if (pthread_mutex_lock(&mutex) != 0)
    {
        perror("ERROR AL BLOQUEAR HILO\n");
        pthread_exit(NULL);
    }
    
    printf("DE LA CAMISETA TIPO: %d, HAY %d CAMISETAS, Y NOS COMPRAN: %d\n", k, camisetas[k], NCompra);    
    if (camisetas[k] > NCompra)
    {
        camisetas[k] = camisetas[k] - NCompra;
        printf("HAY %d CAMISETAS\n", camisetas[k]);
    }
    else if (camisetas[k] <= NCompra)
    {
        camisetas[k] = 0;
        printf("HAY %d CAMISETAS\n", camisetas[k]);
    }

    if (pthread_mutex_unlock(&mutex) != 0)
    {
        perror("ERROR AL DESBLOQUEAR HILO\n");
        pthread_exit(NULL);
    }
}

void *venta(void *N)
{
    int NCamisetas = *(int *)N;
    int k = rand() % (NCamisetas + 1);
    int NVenta = rand() % (100 - 1 + 1) + 1;
    printf("DE LA CAMISETA TIPO: %d, HAY %d CAMISETAS, Y COMPRAMOS: %d\n", k, camisetas[k], NVenta);
    if (pthread_mutex_lock(&mutex) != 0)
    {
        perror("ERROR AL BLOQUEAR HILO\n");
        pthread_exit(NULL);
    }
    camisetas[k] = camisetas[k] + NVenta;
    printf("HAY %d CAMISETAS\n", camisetas[k]);
    if (pthread_mutex_unlock(&mutex) != 0)
    {
        perror("ERROR AL DESBLOQUEAR HILO\n");
        pthread_exit(NULL);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        perror("ERROR AL INTRODUCIR LOS DATOS\n");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));
    int n = atoi(argv[1]); // numero clientes
    int m = atoi(argv[2]); // numero camisetas=numero de provedores
    camisetas = malloc(sizeof(int) * m);

    for (int i = 0; i < m; i++)
    {
        camisetas[i] = rand() % (100 - 1 + 1) + 1;
        printf("CAMISETA NUMERO %d: %d\n", i, camisetas[i]);
    }

    pthread_t hm[m];
    pthread_t hn[n];

    for (int i = 0; i < n; i++)
    {
        if (pthread_create(&hn[i], NULL, (void *)compra, (void *)&m) != 0)
        {
            perror("ERROR AL CREAR EL HILO DE CLIENTES\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < m; i++)
    {
        if (pthread_create(&hm[i], NULL, (void *)venta, (void *)&m) != 0)
        {
            perror("ERROR AL CREAR EL HILO DE PROVEDORES\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (pthread_join(hn[i], NULL) != 0)
        {
            perror("ERROR AL RECOGER EL HILO DE CLIENTES\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < m; i++)
    {
        if (pthread_join(hm[i], NULL) != 0)
        {
            perror("ERROR AL RECOGER EL HILO DE CLIENTES\n");
            exit(EXIT_FAILURE);
        }
    }
}
