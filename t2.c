#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <crypt.h>
#include <string.h>


char SALT[6]= "$6$AS$";

pthread_mutex_t lock;
char salt_with_password[92] = "";
struct PosData
{
    int init;
    int end;
};
int flag = 0;
int count = 0; // A counter used to track the number of combinations explored so far
/**
 Required by lack of standard function in C.   
*/
void substr(char *dest, char *src, int init, int length)
{
    memcpy(dest, src + init, length);
    *(dest + length) = '\0';
}
void *crack(void *position)
{

    int x, y, z;   // Loop counters
    char salt[7];  // String used in hashing the password. Need space for \0 // incase you have modified the salt value, then should modifiy the number accordingly
    char plain[7]; // The combination of letters currently being checked // Please modifiy the number when you enlarge the encrypted password.
    char *enc;     // Pointer to the encrypted password
    struct PosData pos = *(struct PosData *)position;
    substr(salt, salt_with_password, 0, 6);
    for (x = pos.init; x <= pos.end; x++)
    {
        for (y = 'A'; y <= 'Z'; y++)
        {
            for (z = 0; z <= 99; z++)
            {
                sprintf(plain, "%c%c%02d", x, y, z);

                pthread_mutex_lock(&lock);

                enc = (char *)crypt(plain, SALT);
                count++;
                if (strcmp(salt_with_password, enc) == 0)
                {
                    printf("password found #%-8d%s %s\n", count, plain, enc);
                    exit(0);
                }

                pthread_mutex_unlock(&lock);
            }
        }
    }
}
void main(int *argc, char *argv[])
{

    pthread_mutex_init(&lock, NULL);
    int nThreads = atoi(argv[1]);
    pthread_t threadId[nThreads];
    int division = 25 / nThreads;
    int remainder = 25 % nThreads;
    int init = 65, end = 65;
    struct PosData s[nThreads];
    char input[92];
    sprintf(salt_with_password, argv[2]);
    for (int i = 0; i < nThreads; i++)
    {
       s[i].init = init;
        s[i].end = end + division;
        if (remainder > 0)
        {
            s[i].end += 1;
            remainder--;
        }
        init = s[i].end + 1;
        end = s[i].end;
        printf("start for %d is %c and end is %c\n", i, s[i].init, s[i].end);

    }

    for (int j = 0; j < nThreads; j++)
    {

        pthread_create(&threadId[j], NULL, crack, (void *)&s[j]);
    }
    for (int k = 0; k < nThreads; k++)
    {
        pthread_join(threadId[k], NULL);
    }
}