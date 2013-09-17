#include <stdio.h>
#include <math.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_linalg.h>
#include<gsl/gsl_blas.h> 
 

void main(int argc,char **argv){

 //poner error de que flata el argumento para leer


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

  int i,j;
  // guarda los datos en las matrices

  x=malloc(sizeof(float)*filas);

  y=malloc(sizeof(float)*filas);

  printf("se guardan los datos\n"); 

  for(i=0;i<filas;i++){
    fscanf(file,"%lf %lf", &x[i],&y[i]);
    }
 printf("se crearon las matrices de datos \n");

  // se crea el arreglo de datos de tamaño filas*(numero+1)

  int pos;

  //matriz
  double *m1;

  double *mt1;

  int columnas=numero+1;

  m1=malloc(sizeof(float)*filas*(columnas));
  mt1=malloc(sizeof(float)*filas*(columnas));

  printf("se le da memoria a m1 y mt1  \n");
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
   
   printf("m1 y mt1 creadas\n");
 
  //se crea la matriz en gsl}
  //el numero de columnas es igual al numero de entrada

  gsl_matrix_view m= gsl_matrix_view_array (m1, filas, columnas);
  gsl_matrix_view mt= gsl_matrix_view_array (mt1, columnas, filas);

  printf("m1 y mt1 siendo vistas por gsl") ;

  //vector d con entradas dadas por el arreglo Y
  gsl_matrix_view vect=gsl_matrix_view_array(y,filas,1);

  printf("se creo vect\n");

  //necesario para la descomposicion LU
  int s;
  //  gsl_matrix * Multi = gsl_matrix_alloc(m,n1);

  gsl_matrix * Multi = gsl_matrix_alloc(columnas,columnas);
  gsl_matrix * prueva = gsl_matrix_alloc(columnas,columnas);

  printf("se crearon las matrices multi y prueba\n");

  // matriz inversa
  gsl_matrix * Inver = gsl_matrix_alloc(columnas,columnas);

  //matriz total 
  gsl_matrix * Total = gsl_matrix_alloc(columnas,filas);

  printf("se crea la matriz total\n");

  //Multi y prueva son la multiplicacion de m y mt
  gsl_matrix * VECT;
  VECT=&vect.matrix;
  gsl_matrix * MT= gsl_matrix_alloc(columnas,filas);
  MT=&mt.matrix;
  gsl_matrix * M= gsl_matrix_alloc(filas,columnas);
  M=&m.matrix;

  printf("se le asignan las matrices \n");


  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, MT, M, 0.0, Multi);
  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, MT, M, 0.0, prueva);

  // matriz de permutacion necesaria para LU
  gsl_permutation * perm = gsl_permutation_alloc (columnas);
  
  printf("permutacion \n");
  
  
  // se invierte la matriz de prueva que es 
  gsl_linalg_LU_decomp(prueva,perm,&s);
		       
  //Invierte la matriz prueva y guarda el resultado en Inver

  gsl_linalg_LU_invert (prueva, perm, Inver);

  printf("LU decomp\n");
			
  //se guarda la matriz total en total
  //se tien(GtG)⁻1*Gt		      

  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, Inver, MT, 0.0,Total);
   
  // se crea una matriz de resultado (m) y se hace el calculo---------------------------------------------
  gsl_matrix * Resultado = gsl_matrix_alloc(columnas,1);

  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, Total, VECT, 0.0, Resultado);
  	
  //se saca la matriz que tiene en sus posiciones F(Xi)---------------------------------------------------

  gsl_matrix * FuncionXi= gsl_matrix_alloc(filas,1);

  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, M, Resultado, 0.0, FuncionXi);
			
  // sacar shi
 		
  double shi=0;			
			//gsl cuenta desde 1 o 0??

  for(i=0;i<filas;i++){
    printf("%d\n",i);
    printf("%lf\n",gsl_matrix_get(FuncionXi, i, 0));
    shi=shi+ pow((y[i]- gsl_matrix_get(FuncionXi, i, 0)),2)/filas;
  }

// imprime resultado 
	     
   for(i=0;i<filas;i++){

     printf("m%d = %lf\n",i,gsl_matrix_get(Resultado,i,0));

   }
			
   printf("X²=%lf\n",shi);

  
}


