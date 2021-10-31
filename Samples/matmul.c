#include <stdio.h>

#define M 4
#define N 3

void main()
{
  double A[M][N], B[N][M], C[M][M];	

  for(int i=0; i< M; i++){
    for(int j=0; j< N; j++){    
      A[i][j]=i+j+1;
      B[j][i]=(i+1)*(j+1);
    }
  }
  for(int i=0; i< M; i++){
    for(int j=0; j< M; j++){    
      C[i][j] = 0.0;
    }
  }
  
  for(int i=0; i<M; i++){
    for(int j=0; j<M; j++){    
      for(int k=0; k<N; k++){
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  
  for(int i=0; i< M; i++){
    for(int j=0; j< M; j++){
      printf("%lf\t",C[i][j]);
    }
    printf("\n");
  }
} 

