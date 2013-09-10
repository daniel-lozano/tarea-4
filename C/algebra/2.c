#include <stdio.h>
int cambia(int a){
  a=2;

}
int  main(){
  int i =0;
  int j=i;
  i=cambia(i);

  printf("%d es i y %d es j \n",i,j);
}
