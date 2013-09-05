#include <math.h>
#include <stdio.h>
void main(int argc,char **argv){
  int i=1;
  int numero=atoi(argv[1]);
  double factorial=1;
  if(numero<=0){
    printf("el numero debe ser mayor que cero");
    exit(1); }
 
 for(i=1;i<numero+1;i++){
    factorial=factorial*i;
  }
 printf("el factorial del numero %d es %e \n",numero,factorial);
}
