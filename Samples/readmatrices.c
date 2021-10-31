#include <stdio.h>

void main()
{
  FILE *fp = NULL;
  int row, col;
  int rows, cols;
  float matval = 0.0;
  char c;

  fp = fopen("SampleMatricesWithErrors.txt", "r");
  while(!feof(fp)){
    fscanf(fp,"%d,%d",&rows, &cols);
    printf("Rows: %d, Cols: %d\n", rows, cols);
    for(row = 0; row < rows; row++){
      printf("[ ");
      for(col = 0; col < cols-1; col++){
        fscanf(fp,"%f,",&matval);
        printf("%f\t",matval);
      }
      fscanf(fp,"%f\n",&matval);
      printf("%f ]\n",matval);
    }
  }
  fclose(fp);
}

