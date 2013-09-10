#include <stdio.h>
#include <math.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_linalg.h>
#include<gsl/gsl_blas.h> 
 

void main(int argc,char **argv){

 //poner errores

  int Fl=atof(argv[2]);
  int numero= atoi(argv[2]);

  if(numero<=0){
    printf("el númerpo debe ser mayor que cero \n");
    exit(1);}

  if(Fl%1){
    printf("el número debe ser entero \n");
    exit(1);
  }
  FILE *file;

  file=fopen(argv[1],"r");

  double *x;
  double *y;

  //cuenta el numero de filas en el archivo

  int filas=0;
  int fg=0;

  do{
    fg=fgetc(file) ;
    if(fg=='\n'){
      filas++;
    }
  }while(fg!=EOF);
  rewind(file);
  printf("el numero de filas es %d \n", filas);


  // guarda los datos en las matrices

  x=malloc(sizeof(float)*filas);

  y=malloc(sizeof(float)*filas);
 
  for(i=0;i<filas;i++){
    fscanf(file,"%e %e", &x[i],&y[i]);
    }

  // se crea el arreglo de datos de tamaño filas*(numero+1)

  int pos;

  //matriz
  float *m1;
  float *mt1;
  int columnas=numero+1;
  m1=malloc(sizeof(float)*filas*(columnas));
  mt1=malloc(sizeof(float)*filas*(columnas));

  //crea m1
  for(i=0;i<filas;i++){

    for(j=0;j<columnas;j++){

      pos=j+(filas*i);
      m1[pos]=pow(x[i],j);
   }
  } 

  //crea mt1
   for(i=0;i<filas;i++){

    for(j=0;j<columnas;j++){

      pos=i+(filas*j);
      mt1[pos]=pow(x[i],j);
   }
  } 
 
  //se crea la matriz en gsl}

  gsl_matrix_view m= gsl_matrix_view_array (m1, filas, columnas);
  gsl_matrix_view mt= gsl_matrix_view_array (mt1, filas, columnas);

  gsl_matrix * C = gsl_matrix_alloc(m,n1);
 
  //C es la multiplicacion de m y mt


  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, mt, m, 0.0, C); 
  

  
}
