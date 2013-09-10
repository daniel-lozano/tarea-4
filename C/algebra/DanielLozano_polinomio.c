#include <stdio.h>


void main(int argc,char **argv){

  int numero= atoi(argv[2]);
  FILE *file;
  file=fopen(argv[1],"r");
  
  printf("%d",numero);
  

}
