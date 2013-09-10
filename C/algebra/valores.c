#include <stdio.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>

void main(int argc, char **argv){
  File *file;
  int *posiciones;
  float *y;
  float *t;

  file=fopen(argv[1],"r");
  y=malloc(sizeof(float)*100);
  t=malloc(sizeof(float)*100);
  int i,j;

  for(i=0;i<100;i++){
    fscanf(file,"%e %e",&t[i],&y[i]);  
  }
  
  // creacion de la matriz 
  float *matriz;

  matriz=malloc(sizeof(float)*100*3);
 //si la matriz existe

  int pos;
  if(!matriz){
    printf("hubo problemas guardando la matriz");
    exit(1);}

 //se creo la matriz

  for(i=0;i<100;i++){
    for(j=0;j<3;j++){
      pos=j+(i*100);

      if(j==0){
	matriz[pos]=1;
      }
      if(j==1){
	matriz[pos]=t[i];
      }
      if(j==2){
	matriz[pos]=t[i]*t[i];
      }
    }

  }

  //gsl_matrix_view m
  //   = gsl_matrix_view_array (matriz, 100, 3);
 /*
  gsl_vector *eval = gsl_vector_alloc (4);
  gsl_matrix *evec = gsl_matrix_alloc (4, 4);
  
  gsl_eigen_symmv_workspace * w = gsl_eigen_symmv_alloc (4);
  
  gsl_eigen_symmv (&m.matrix, eval, evec, w);
  
  gsl_eigen_symmv_free (w);
  
  gsl_eigen_symmv_sort (eval, evec,
GSL_EIGEN_SORT_ABS_ASC);
 **/






}
