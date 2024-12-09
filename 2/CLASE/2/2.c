#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int funcion(char*cadena){
    FILE * f1;
    int n1=0;
    f1=fopen(cadena, "r");  
    char buffer[100];  
    while (fgets(buffer, 100 , f1) != NULL) {
        n1++;
    }
    int * res=malloc(sizeof(int));
    *res=n1++;
    fclose(f1);
    pthread_exit((void*)res);
}

int main(int argc, char **argv){
    int sum=0;
    int * res;
    char aux[100];
    pthread_t tid[argc-1];
    for(int i=0; i<argc-1; i++){
        if( pthread_create(&tid[i], NULL, (void *)funcion, (void*)argv[i+1]))
        {
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }
    for(int j=0; j<argc-1; j++){
        if(pthread_join(tid[j], (void**)&res))
        {
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);
        }
        sum=sum+(*res);
    }
    printf("En total hay %d lineas\n", sum);
}