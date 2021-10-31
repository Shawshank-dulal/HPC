#include <stdio.h>

#define M 4
#define N 3

void main()
{
  double A[M*N], B[N*M], C[M*M];	

  printf("Matrix A\n");
  for(int i=0; i< M; i++){
    for(int j=0; j< N; j++){    
      A[(i*N)+j]=i+j+1;
      printf("%f\t",A[(i*N)+j]);
    }
    printf(" ]\n");
  }
  printf("Matrix B\n");
  for(int j=0; j< N; j++){    
    for(int i=0; i< M; i++){
      B[(j*M)+i]=(i+1)*(j+1);
      printf("%f\t",B[(j*M)+i]);
    }
    printf(" ]\n");
  }
  for(int i=0; i< M; i++){
    for(int j=0; j< M; j++){    
      C[(i*M)+j] = 0.0;
    }
  }
  
  for(int i=0; i<M; i++){
    for(int j=0; j<M; j++){    
      for(int k=0; k<N; k++){
        C[(i*M)+j] += A[(i*N)+k] * B[(k*M)+j];
      }
    }
  }
  
  printf("Matrix C\n");
  for(int i=0; i< M; i++){
    for(int j=0; j< M; j++){
      printf("%lf\t",C[(i*M)+j]);
    }
    printf("\n");
  }
} 

