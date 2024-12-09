#include <pthread.h> 
#include <stdlib.h> 
#include <stdio.h>
#include <unistd.h> 


pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; 

int cliente(int * v){
    int n=rand()%6;
    int s;
    s = pthread_mutex_lock(&mtx); 
    if (s != 0){
        printf("mutex_lock error...\n");
        pthread_exit(NULL); 
    }    
    int aux=v[n];
    if((v[n]=v[n]-rand()%11)<=0){
        v[n]=aux;
    }
    s = pthread_mutex_unlock(&mtx);
    if (s != 0){
		printf("mutex_unlock error...\n");
		pthread_exit(NULL); 
	}
}
int proveedor(int * v){
    int n=rand()%6;
    int s = pthread_mutex_lock(&mtx); 
    if (s != 0){
        printf("mutex_lock error...\n");
        pthread_exit(NULL); 
    }  
    v[n]=v[n]+rand()%11;
    s = pthread_mutex_unlock(&mtx);
    if (s != 0){
		printf("mutex_unlock error...\n");
		pthread_exit(NULL); 
	}
}

int main(int argc, char*argv[]){
    if(argc!=3){
        perror("ERROR. primero clientes luego provedores");
        exit(EXIT_FAILURE);
    }
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    pthread_t hn[n];
    pthread_t hm[m];

 

    int * v=malloc(sizeof(int)*m);

    for(int i=0; i<m; i++){
        v[i]=rand()%101;
    }

    for(int i=0; i<n; i++){
        if(pthread_create(&hn[i], NULL, (void*)cliente, (void*)v)!=0){
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }
    for(int i=0; i<m; i++){
        if(pthread_create(&hm[i], NULL, (void*)proveedor , (void*)v)!=0){
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }
    for(int j=0; j<n; j++){
        if(pthread_join(hn[j], NULL))
        {
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);
        }
    }
    for(int j=0; j<m; j++){
        if(pthread_join(hm[j], NULL))
        {
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);
        }
    }
    for(int i=0; i<m; i++){
        printf("%d\n", v[i]);
    }
}
