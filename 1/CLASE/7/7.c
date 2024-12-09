/*Realizar un programa que capture la señal de alarma, de manera que imprima la cadena “RING”
pasados 5 segundos, después pasados otros 3 segundos y por último cada segundo. Implementar
esto último, utilizando un bucle infinito que vaya imprimiendo el número de timbrazos. Pasados 4
timbrazos, el proceso se debe parar utilizando para ello la función kill().*/
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>

void sig_alrm(){
    printf("RING\n");//por que sin \n no funciona
    return;
}
void f(int secs){
    alarm(secs);
    pause();
}

int main(){
    int i=0;
    if (signal(SIGALRM, sig_alrm) == SIG_ERR){
		perror("Signal error");
   	    printf("errno value= %d\n", errno);  
		exit(EXIT_FAILURE);
	}
    printf("alarma en 2\n");
    f(2);
    printf("alarma en 2\n");
    f(2);
    printf("\n");
        while(i!=1){
        f(1);
   }
}