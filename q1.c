#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

FILE *fp;
int position;
int ind = 1;
void divideWorkload()
{
}

void readMatrices()
{
    FILE *fp = NULL;
    int row, col;
    int rows, cols;
    float matval = 0.0;
    int col1, row2;

    fp = fopen("SampleMatricesWithErrors.txt", "r");
    for (int ind = 0; ind < position * 2 - 2; ind++)
    {
        fscanf(fp, "%d,%d", &rows, &cols);
        // printf("Rows: %d, Cols: %d\n", rows, cols);
        for (row = 0; row < rows; row++)
        {
            for (col = 0; col < cols - 1; col++)
            {
                fscanf(fp, "%f,", &matval);
            }
            fscanf(fp, "%f\n", &matval);
        }
    }

    float matrixArr[rows * cols];
    float matrix1[rows][cols], matrix2[cols][rows];
    int count = 1;

    if (count == 1)
    {
        fscanf(fp, "%d,%d", &rows, &cols);
        col1 = cols;
        printf("Matrix 1: Rows: %d, Cols: %d\n", rows, cols);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                fscanf(fp, "%f,", &matrix1[i][j]);
            }
            fscanf(fp, "\n");
        }
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                printf("%f\t", matrix1[i][j]);
            }
            printf("\n");
        }
        count++;
    }
    if (count == 2)
    {
        fscanf(fp, "%d,%d", &rows, &cols);
        printf("Matrix 2: Rows: %d, Cols: %d\n", rows, cols);

        if (col1 != rows)
        {
            printf("this matrix combination cannot be multiplied");
        }
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                fscanf(fp, "%f,", &matrix1[i][j]);
            }
            fscanf(fp, "\n");
        }
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                printf("%f\t", matrix1[i][j]);
            }
            printf("\n");
        }
    }
}

void main(int argc, char *argv[])
{
    // int nThreads = atoi(argv[1]);
    // int a[]={{1,2,3},{2,4,6},{3,6,9}};
    // int b[]={{1,2,3,5},{2,4,6,7},{3,6,9,8}};

    // int requiredCalculations=fileread();

    // if(nThreads>requiredCalculations){
    //     nThreads=requiredCalculations;
    // }

    // for(int i=0;i<nThreads;i++){
    //     pthread_create(&threadId[j], NULL, primecalc, (void *)&s1[j]);
    // }
    printf("plese enter which matrix combination you want to use:");
    scanf("%d", &position);
    readMatrices();
}