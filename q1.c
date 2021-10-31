#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

FILE *fp;

void divideWorkload(){
    
}


// void fileread() //returns maximum number of computations required
// {       
//     for (int i = 0; i < 4; i++)
//     {
//         char loc[50] = {"./LinearRegressionData/"};
//         strcat(loc, filename[i]);
//         fp = fopen(loc, "r");
//         if (fp == NULL)
//         {
//             printf("Error in opening file");
//         }

//         while (!feof(fp))
//         {
//             int a, b;
//             fscanf(fp, "%d,%d\n", &a, &b);
//             x[end] = a;
//             y[end] = b;
//             end++;
//             total++;
//         }
//         fclose(fp);
//     }

// }



void main(int argc, char* argv[]){
    int nThreads = atoi(argv[1]);
    int a[]={{1,2,3},{2,4,6},{3,6,9}};
    int b[]={{1,2,3,5},{2,4,6,7},{3,6,9,8}}; 

    int requiredCalculations=fileread();

    if(nThreads>requiredCalculations){
        nThreads=requiredCalculations;
    }
    
    for(int i=0;i<nThreads;i++){
        pthread_create(&threadId[j], NULL, primecalc, (void *)&s1[j]);
    }

    
}