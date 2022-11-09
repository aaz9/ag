#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

//Global variable
int a= 10;

//Variable for reader count
int readcount = 0;

//Mutex variables
pthread_mutex_t wr, mutex;

//Reader function
void * reader(void * arg)
{
    long int num;
    num = (long int)arg;

    //Reader implementing lock so that one reader at a time reads
    pthread_mutex_lock(&mutex);
        readcount++;
    
    //Unlocking the variable
    pthread_mutex_unlock(&mutex);

    //If readcount is 1, then lock the writer
    if(readcount == 1)
    {
        pthread_mutex_lock(&wr);
    }
    printf("\nReader %ld is in critical section", num);

    printf("\nReader %ld is reading data %d", num, a);
    sleep(1);

    //Leaving critical section
    pthread_mutex_lock(&mutex);
    readcount--;
    pthread_mutex_unlock(&mutex);

    //If readcount is 0, unlock the writer
    if(readcount == 0)
    {
        pthread_mutex_unlock(&wr);
    }

    printf("\nReader %ld leaves critical section", num);
}
//End of reader function

//Writer function
void * writer(void * arg)
{
    long int num;
    num = (long int)arg;

    //Lock wr variable to enter critical section
    pthread_mutex_lock(&wr);
    printf("\nWriter %ld is in critical section", num);
    printf("\nWriter %ld has written data as: %d", num, ++a);
    sleep(1);

    //Writer releasing the lock
    pthread_mutex_unlock(&wr);
    printf("\nWriter %ld leaves critical section", num);
}
//end of writer function

//Main function
int main()
{
    //thread variables where r = reader and w = writer
    pthread_t r[10], w[10];
    
    //Index variables for creating and joining threads
    long int i,j;

    //Variables to stor number of readers and writers
    int nor, now;

    //Initializing mutex variables
    pthread_mutex_init(&wr, NULL);
    pthread_mutex_init(&mutex, NULL);
    
    //Asking number of readers and writers from the user
    printf("Enter number of readers and writers:\n");
    scanf("%d %d", &nor, &now);

    //Creating reader and writer threads
    for(i=0; i<nor; i++)
    {
        pthread_create(&r[i], NULL, reader, (void *)i);   //Creating reader thread
    }
    for(j=0; j<now; j++)
    {
        pthread_create(&w[i], NULL, writer, (void *)j);   //Creating writer thread
    }

    //Joining the threads
     for(i=0; i<nor; i++)
    {
        pthread_join(r[i], NULL);   //Joining reader thread
    }
    for(j=0; j<now; j++)
    {
        pthread_join(w[j], NULL);   //Joining writer thread
    }
    return 0;
}
//end of main function
