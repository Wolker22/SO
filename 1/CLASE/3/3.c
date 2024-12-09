#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h>
#include <sys/wait.h>
 
 int main(int argc, char**argv){
    int status;
    pid_t pid, wpid;
    if(argc!=2){
        perror("ERROR\n");
        exit(EXIT_FAILURE);
    }

    int n=atoi(argv[1]);

    for(int i=0; i<n; i++){
        pid =fork();
        if(pid ==-1){
            perror("ERRRORO");
            exit(EXIT_FAILURE);
        }         
        sleep(n);
        if(pid==0){

          printf("Soy el hijo mi pid %d y el pid de mi padre es %d\n", getpid(), getppid());           

          exit(EXIT_SUCCESS);
        }            

    }

    wpid = waitpid(pid, &status, WUNTRACED);
    if(pid>0)
      {
        if (WIFEXITED(status)) //si la funcion WIFEXITED retorna con exito se cumple la condición, esto ocurre so se ha completado la ejecucion sin interrupciones
        {
            printf("Soy el padre %d, hijo %d recogido, status=%d\n",getpid(), pid, WEXITSTATUS(status));
        } 
        else if (WIFSIGNALED(status)) //si la funcion WIFSIGNALED retorna con exito si el proceso hijo terminó debido a una señal en lugar de una salida normal.
        {
            printf("Soy el padre, hijo %d matado (signal %d)\n", pid, WTERMSIG(status));
        } 
      }
    else 
      {
        printf("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal.\n");
        exit(EXIT_FAILURE);
      } 
      exit(EXIT_SUCCESS); //exit 0; return 0;
}





