#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 1000;

FILE *file, *fileStore = NULL;
int row, col;                   // to use in loop counters
int rows1, cols1, rows2, cols2; // to store rows and columns of matrix A & B
int threadCount;


// all the available function down below
void readMatrix();
void *calculateMatrix(void *arg);
void resultantMatrix(double *A[rows1], double *B[rows2]);

void main(int argc, char **argv)
{
    threadCount = atoi(argv[1]);
    if (threadCount <= 0 || threadCount >= MAX_THREADS)
    {
        printf("ERROR: You can only set threads between 1 - 999 !!\n\n");
        return;
    }

    readMatrix();
    printf("Done.\n");
    fclose(file);
    fclose(fileStore);
}

// To read all the matrix A and B from the file.
void readMatrix()
{
    fileStore = fopen("matrixresults2049699.txt", "w");
        file = fopen("Data/SampleMatricesWithErrors2.txt", "r");
        double matval = 0.0;
        while (fscanf(file, "%d,%d", &rows1, &cols1) != EOF)
        {
            // To store first matrix A
            double *A[rows1];
            for (row = 0; row < rows1; row++)
            {
                // to dynamically allocate memory
                A[row] = malloc(cols1 * sizeof(double *));
                for (col = 0; col < cols1; col++)
                {
                    fscanf(file, "%lf,", &matval);
                    A[row][col] = matval;
                }
            }

            // To store second matrix B
            fscanf(file, "%d,%d", &rows2, &cols2);
            double *B[rows2];
            for (row = 0; row < rows2; row++)
            {
                B[row] = malloc(cols2 * sizeof(double *));
                for (col = 0; col < cols2; col++)
                {
                    fscanf(file, "%lf,", &matval);
                    B[row][col] = matval;
                }
            }

            // if matrix cannot be multiplied like 3x3 by 2x2 then break the iteration
            if (cols1 != rows2)
            {
                printf("Matrix A is %dx%d\n", rows1, cols1);
                printf("Matrix B is %dx%d\n", rows2, cols2);
                printf("Error Message:: The matrix A and B cannot be multiplied.\n\n");
                continue;
            }

            resultantMatrix(A, B);
        }
}

// slicing for threads and perform calculation for resultant matrix
void resultantMatrix(double *A[rows1], double *B[rows2])
{

    double *data = NULL;
    int nThreads = threadCount, count = 0, breakCounter = 0;
    pthread_t threads[nThreads];
    int test = 1; // optional

    // if user requested number greater than the biggest dimension of the matrices
    if (nThreads > rows1 && nThreads > cols2)
        nThreads = rows1 > cols2 ? rows1 : cols2;

    // To store and perform matrix multiplication
    fprintf(fileStore, "Resultant Matrix: %d,%d\n", rows1, cols2);
    double C[rows1][cols2];
    for (row = 0; row < rows1; row++)
    {
        for (col = 0; col < cols2; col++)
        {
            // storing 1st mat row and 2nd mat column
            // (cols1 * row2 +1) OR (cols1 * 2 +1) same
            data = malloc((cols1 * 2 + 1) * sizeof(double));
            data[0] = cols1;

            int k;
            for (k = 0; k < cols1; k++)
                data[k + 1] = A[row][k];

            for (k = 0; k < rows2; k++)
                data[k + cols1 + 1] = B[k][col];

            if (count < nThreads)
            {
                pthread_create(&threads[count], NULL, calculateMatrix, (void *)data);
                count++;
            }

            if (count == nThreads || row + col == rows1 + cols2 - 2)
            {
                //printf("%d\n", test++);
                for (int i = 0; i < count; i++)
                {
                    void *ptr;
                    pthread_join(threads[i], &ptr);
                    fprintf(fileStore, "%lf\t", *(double *)ptr);

                    // to break into new line after each column end.
                    breakCounter++;
                    if (breakCounter == cols2)
                    {
                        fprintf(fileStore, "\n");
                        breakCounter = 0;
                    }
                }
                count = 0;
            }
        }
    }
    fprintf(fileStore, "\n");
}

// using thread for matrix multiplication
void *calculateMatrix(void *arg)
{
    double *data = (double *)arg;
    double finalValue = 0.0;
    int index = data[0], z = 0;

    for (z = 1; z <= index; z++)
        finalValue += data[z] * data[z + index];

    double *last = malloc(sizeof(double));
    *last = finalValue;
    pthread_exit(last);
}
    fp = fopen("SampleMatricesWithErrors4.txt", "r");
