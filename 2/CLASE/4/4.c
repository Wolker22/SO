#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>


int mifuncion(int * n){
    *n=*n+10000;
    return *n;
}

int main(){
    int *n=malloc(sizeof(int));
    int res;
    pthread_t v[2];
    for(int i=0; i<2; i++){
        if(pthread_create(&v[i], NULL, (void*)mifuncion , (void*)n)!=0){
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }
    for(int j=0; j<2; j++){
        if(pthread_join(v[j], (void**)&res))
        {
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);
        }
}
printf("res: %d\n", res);
}