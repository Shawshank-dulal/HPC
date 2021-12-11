#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

FILE *fp, *outputFile = NULL;
int row, col, row1, col1, row2, col2, NUM_THREADS, next_Break = 0, s = 0, counter = 0, i = 0;
double *a;
double mat;

void *matMulThreads(void *arg)
{
    double *values = (double *)arg;
    int index = values[0], i = 0, j = 0, z = 1;
    a = malloc(sizeof(double));

    double return_Values = 0.0;
    while (z <= index)
    {
        return_Values += values[z + index] * values[z];
        z++;
    }
    *a = return_Values;
    pthread_exit((void *)a);
}
void checking_Threads(int n_Thread)
{
    if (n_Thread > row1)
    {
        if (n_Thread > col2)
        {
            if (row1 > col2)
            {
                n_Thread = row1;
            }
            else
            {
                n_Thread = col2;
            }
        }
    }
}

// performing matrix multiplication in threds concurrently
void matrixMultiplication(double *A[row1], double *B[row2])
{
    double *values = NULL;
    int n_Thread = NUM_THREADS;

    checking_Threads(NUM_THREADS);
    fprintf(outputFile, "%d,%d\n", row1, col2);

    double C[row1][col2];
    pthread_t threads[n_Thread];

    row = 0;
    while (row < row1)
    {
        col = 0;
        while (col < col2)
        {
            values = malloc((col1 * 2 + 1) * sizeof(double));

            values[0] = col1;
            // first and second data in values
            s = 0;
            while (s < col1)
            {
                values[s + 1] = A[row][s];
                values[s + 1 + col1] = B[s][col];
                s++;
            }

            if (counter < n_Thread)
            {
                pthread_create(&threads[counter], NULL, matMulThreads, (void *)values);
                counter++;
            }

            int check = (row + col == row1 + col2 - 2 || counter == n_Thread);
            if (check)
            {
                i = 0;
                while (i < counter)
                {
                    void *ptr_values;
                    pthread_join(threads[i], &ptr_values);
                    fprintf(outputFile, "%lf  ", *(double *)ptr_values);

                    next_Break++;

                    if (next_Break == col2)
                    {
                        fprintf(outputFile, "\n");
                        next_Break = 0;
                        i++;
                    }
                    else
                    {
                        i++;
                    }
                }
                counter = 0;
            }
            col++;
        }
        row++;
    }
    fprintf(outputFile, "\n");
}

void readMatrices()
{
    double matrix;
    fp = fopen("SampleMatricesWithErrors.txt", "r");
    outputFile = fopen("matrixresults2049822.txt", "w");

    while (fscanf(fp, "%d,%d", &row1, &col1) != EOF)
    {
        // read first matrix
        double **A;
        for (row = 0; row < row1; row++)
        {
            A[row] = (double *)malloc(col1 * sizeof(double));
            for (col = 0; col < col1; col++)
            {
                matrix = 0.0;
                fscanf(fp, "%lf,", &matrix);
                A[row][col] = matrix;
            }
        }

        // read second matrix
        fscanf(fp, "%d,%d", &row2, &col2);
        double *B[row2];
        for (row = 0; row < row2; row++)
        {
            B[row] = (double *)malloc(col2 * sizeof(double));
            for (col = 0; col < col2; col++)
            {
                matrix = 0.0;
                fscanf(fp, "%lf,", &matrix);
                B[row][col] = matrix;
            }
        }

        // in m x n matrix and n x p matrix if ns are not the same break the loop.
        if (col1 != row2)
        {
            printf("The two matrix cannot be multiplied.\n");
            continue;
        }

        matrixMultiplication(A, B);
    }
}

int main(int argc, char **argv)
{
    NUM_THREADS = atoi(argv[1]);
    if (NUM_THREADS <= 0 || NUM_THREADS > 999)
    {
        printf("agrument values must be between 0 and 1000!!\n");
        return 0;
    }

    readMatrices();
    fclose(fp);

    return 0;
}