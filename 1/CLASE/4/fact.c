#include <stdio.h>
#include <stdlib.h>
int main(int argc, char * argv[]){
    int n, result;
    n=atoi(argv[1]);
    for(int i=1; i<=n; i++){
        result=result*i;
    }
    printf("El resultado de %d es: %d.", n, result);
}