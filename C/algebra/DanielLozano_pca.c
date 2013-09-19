#include <stdio.h>
#include <math.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_linalg.h>
#include<gsl/gsl_blas.h> 
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_eigen.h>

int main (int argc,char **argv){
  // para correr -lm -lgsl -lgslcblas

  FILE *file;
  int columnas=24;
  int i,j,k;
  int filas=0;
  int fg=0;
  double *pro;
  double total;
  double suma;
  double ve=10;
  


  printf("iniciando programa...\n");
 
  if(argc<2){
    printf("ingrese un archivo para comenzar\n");
    exit(1);}

  file=fopen(argv[1],"r");
  printf("corre1?\n");

  do{
    fg=fgetc(file);
    if(fg=='\n'){
      filas++;
    }
  }while(fg!=EOF);
  rewind(file);
  printf("el numero de filas es %d \n", filas);


  //se tiene el numero de columnas y filas del archivo, con la restriccion de qu   e el archivo siempre tendra 24 columnas

  gsl_matrix * vectores=gsl_matrix_alloc(filas,columnas);

  printf("se guardaran los datos en la matriz vectores...\n");

  gsl_matrix_fscanf(file,vectores);

  printf("datos guardados...\n");

  //imprime los datos

  /* for(i=0;i<columnas;i++){
     for(j=0;j<filas;j++){
     printf("%lf ",gsl_matrix_get(vectores,j,i));}
   printf("\n");
   }*/

  gsl_matrix *co= gsl_matrix_alloc(filas,filas);

  //gsl_matrix_get(FuncionXi, i, 0))
  // matriz de covarianzas
  

  pro=malloc(sizeof(double)*columnas);
  if(!pro){
    fprintf(stdout, "problema con pro\n");
    exit(1);
  }

  //promedio de cada fila

  for(i=0;i<filas;i++){
    total=0;
    for(j=0;j<columnas;j++){      
      total=total+gsl_matrix_get(vectores,i,j)/columnas;
    }
    pro[i]=total;
  }
  printf("promedios\n");
  
  // for(i=0;i<filas;i++){
  // printf("promedio %d %lf\n",i,pro[i]);}

  //se creara la matriz de covarianza
 
  for(i=0;i<filas;i++){   
    for(j=0;j<filas;j++){
      suma =0;
      for(k=0;k<columnas;k++){
	suma=suma+(gsl_matrix_get(vectores,i,k)-pro[i])*(gsl_matrix_get(vectores,j,k)-pro[j])/(columnas-1);
      
      }
      gsl_matrix_set(co,i,j,suma);
    }
  }

  gsl_eigen_symmv_workspace * w = gsl_eigen_symmv_alloc (filas);
  if(!w){
    fprintf(stderr, "problema\n");
    exit(1);
  }
  printf("funciona w? %d\n", filas);

  gsl_vector *eval = gsl_vector_alloc (filas);
  if(!eval){
    fprintf(stderr, "problema\n");
    exit(1);
  }

  FILE *out;
  FILE *out1;
  FILE *out2;
  out = fopen("DanielLozano_eigenVectors.dat","w");
  out1 = fopen("DanielLozano_eigenValues.dat","w");
  out2 = fopen("DanielLozano_pca_chisquare.dat ","w");


  printf("funciona eval?\n");

  printf("la dimension de la matriz evec es de %d x %d\n",filas,filas);

  printf("porque se muere en la alocacion de evec!!\n");
  printf("se tiene ya la matriz de covarizan, los promedios y los datos guardados correctamente\n");

  //problema????????????????????????????????????????

  fprintf(stderr,"funciona evec?\n");

  fflush(stderr);



  gsl_matrix *evec = gsl_matrix_alloc (filas,filas);  
  

  gsl_eigen_symmv (co, eval, evec, w);
 
  gsl_eigen_symmv_sort (eval, evec,GSL_EIGEN_SORT_ABS_DESC);

    
  //se imprimen los vectores propios-----------------------------------------------------------------------------

  for(i=0;i<filas;i++)
    {
    for(j=0;j<ve;j++)
      {
	  fprintf(out,"%lf  ",gsl_matrix_get(evec,i,j));
      }
    fprintf(out,"\n");
    }

  //se imprimen los valores propios

  for(i=0;i<filas;i++){
    fprintf(out1,"%lf ",gsl_vector_get (eval, i));
    
  }
          
  gsl_matrix *As=gsl_matrix_alloc(ve,columnas);
  gsl_matrix *se=gsl_matrix_alloc(filas,ve);
  gsl_matrix *reconstruccion=gsl_matrix_alloc(filas,columnas);
  
  //se crea el arreglo de las 10 primeras señales
    
  for(i=0;i<ve;i++){
    for(j=0;j<filas;j++){
      gsl_matrix_set(se,i,j,gsl_matrix_get(evec,i,j));}
  }
  
  
  //se crea la matriz de los 10 primeros vectores propios
  
  for(i=0;i<columnas;i++){
    //recorre los vectores propios
    
    
    for(j=0;j<ve;j++){
      //recorre las señales inciales
      double suma=0;
      fprintf(out2,"evec%d, vect%d:\n ",j,i);
      
      //producto punto
      for(k=0;k<filas;k++){
	suma=suma+gsl_matrix_get(vectores,k,j)*gsl_matrix_get(evec,k,i);
	  }
      
      //guarda los A en la matriz As
      gsl_matrix_set(As,i,j,suma);
      fprintf(out2,"%lf  ",suma);
      
      suma=0;
    }
    fprintf(out2,"\n");
  }
  
  //se crea la matriz de recontruccion
  
  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, se, As, 0.0, reconstruccion);
  
  printf("se ha hallado X²\n");
  
  //calcular X²-----------------------------------------------------
 
  
  //se crea una matriz que tenga la reconstruccion de las señales con los As correspondientes

 printf("hallando los X²...\n");

 //se halla chi para cad par de vectores recontruccion-real

 for(i=0;i<columnas;i++){

   double suma=0;
   fprintf(out2,"x² de vector %d con su recontrusccion:\n",i);
   for(j=0;j<filas;j++){
     
     suma=suma+pow(gsl_matrix_get(vectores,j,i)-gsl_matrix_get(reconstruccion,j,i),2)/(filas);
     
   }
   fprintf(out2,"%lf",suma);
   suma=0;
 }
 




 gsl_vector_free (eval);
 gsl_matrix_free (evec);
 gsl_matrix_free(se);
 gsl_matrix_free(reconstruccion);
 gsl_matrix_free(As);
 gsl_matrix_free(co); 
     



 return 0;
  
}
