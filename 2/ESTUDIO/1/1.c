/*Implemente un programa que cree un número N de hebras. Cada hebra creará 2 números
aleatorios flotantes (consulte Moodle para la generación de aleatorios) y guardará su suma en una
variable para ello, que será devuelta a la hebra principal o llamadora (la que invocó
pthread_create()).
La hebra principal ira recogiendo los valores devueltos por las N hebras y los ira sumando en
una variable no global cuyo resultado mostrará al final por pantalla. Para ver que los resultados
finales son los que usted espera, muestre los 2 números que va creando cada hebra y su suma, de
forma que pueda comparar esas sumas parciales con la suma final que realizará el main() o hebra
principal. Utilice macros definidas y comprobación de errores en sus programas (errno y
comprobación de valores devueltos en cada llamada, con sus posibles alternativas).*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void *f()
{
    float N1 = drand48();
    printf("Primer numero: %f\n", N1);
    float N2 = drand48();
    printf("Segundo numero: %f\n", N2);
    float *N3 = malloc(sizeof(float));
    *N3 = N2 + N1;
    printf("SUMA: %f\n", *N3);
    pthread_exit((void *)N3);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("error introduce bien los datos\n");
        exit(EXIT_FAILURE);
    }
    int n = atoi(argv[1]);
    srand(time(NULL));
    pthread_t p[n];
    for (int i = 0; i < n; i++)
    {
        if (pthread_create(&p[i], NULL, f, NULL) != 0)
        {
            perror("error al crear el hilo\n");
            exit(EXIT_FAILURE);
        }
    }
    float *sum;
    float t = 0;
    for (int i = 0; i < n; i++)
    {
        if (pthread_join(p[i], (void **)&sum) != 0)
        {
            perror("error al recoger el hilo\n");
            exit(EXIT_FAILURE);
        }
        printf("%d\n", i);
        t = t + *sum;
        free(sum);
    }
    printf("TOTAL: %f\n", t);
    exit(EXIT_SUCCESS);
}