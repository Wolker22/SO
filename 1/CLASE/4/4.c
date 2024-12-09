#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
/*#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>*/

int main(int argc, char**argv){
    pid_t pid, childpid;
    int status;
    for(int i=0; i<2; i++){
        pid=fork();
        switch (pid){
        case -1:
            perror("fork error");
            printf("errno value= %d\n", errno);
            exit(EXIT_FAILURE);
            break;
        case 0:
            printf("Proceso hijo con PID %d; mi padre con PPID %d\n", getpid(), getppid());
            if (execlp("./a.out","./a.out","7",NULL)==-1){
                perror("exec");
                printf("errno value= %d\n", errno); 
                exit(EXIT_FAILURE);
            }
        break;
        default:
            do{
                childpid = waitpid(pid, &status, WUNTRACED);
                if (childpid == -1) {
                    perror("waitpid");
                    exit(EXIT_FAILURE);
                }
                if (WIFEXITED(status)) {
                    printf("child exited, status=%d\n", WEXITSTATUS(status));
                } else if (WIFSIGNALED(status)) {
                    printf("child killed (signal %d)\n", WTERMSIG(status));
                } else if (WIFSTOPPED(status)) {
                    printf("child stopped (signal %d)\n", WSTOPSIG(status));
                } else {    /* Non-standard case -- may never happen */
                printf("Unexpected status (0x%x)\n", status);
                }
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            break;
        }
    }
}
