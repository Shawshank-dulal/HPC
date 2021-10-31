#include <stdio.h>
#include <pthread.h>
#include <ctype.h>

#define NUM_THREADS 200

struct args_t {
  int threadID;
};

void * processData(void *p)
{
  int x,y,z;
  struct args_t *pArgs = p;
  int i = 0;
  printf("Thread %d started\n",pArgs->threadID);
  for(x = 'A'; x <= 'E'; x++){
    for(y = 'A'; y <= 'E'; y++){
      for(z = 'A'; z <= 'E'; z++){
        if(i % NUM_THREADS == pArgs->threadID){
          printf("Thread %d processing '%c%c%c'\n",
            pArgs->threadID, x,y,z);
          usleep(100);
        } 
        i++;
      }
    }
  }
  printf("Thread %d ended\n",pArgs->threadID);
}

void main()
{
  pthread_t tid[NUM_THREADS];
  struct args_t args[NUM_THREADS];
  
  for(int i = 0; i < NUM_THREADS; i++){
    args[i].threadID = i;
    pthread_create(&tid[i],NULL,processData,&args[i]);
  }
  for(int i = 0; i < NUM_THREADS; i++){
    pthread_join(tid[i], NULL);
  }
}  
 
 
