#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>

void tratarSennal(){
    printf("recibiendo del padre\n");
}

int main(){
    pid_t pid;
    int status;
    pid=fork();
    switch(pid){
        case -1:
            perror("fork error");
            printf("errno value= %d\n", errno);
            exit(EXIT_FAILURE);
            break;
        case 0: 
            signal(SIGUSR1, tratarSennal);
            for(int i =0;i<5;i++){
                pause();
            }
            sleep(1);
            exit(EXIT_SUCCESS);
        default:    
            printf("Esperando a que mi hijo acabe \n");
            for (int i = 0; i<5; i++){
                sleep(1);
                kill(pid, SIGUSR1); 
            }     
            sleep(1);
            kill(pid, SIGKILL); 
            wait(&status);
            printf("Hijo finalizado con código de salida %d \n", WEXITSTATUS(status));
    }
}
