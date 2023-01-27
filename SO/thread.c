#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


int counter = 1;
pthread_mutex_t mutex;


void *t1()
{
  pthread_mutex_lock(&mutex);
  printf("soy un thread numero %d \n", counter);
  counter++; 
 pthread_mutex_unlock(&mutex);
  pthread_exit(NULL); //retorna algo
}

int main()
{

  int large = 10;
  pthread_mutex_init(&mutex, NULL);
  pthread_t thread[large];

  for (long i = 0; i < large; i++)
  {

    pthread_create(&thread[i], NULL, t1, NULL);
  }
  for (int i = 0; i < 10; i++)
  {
    pthread_join(thread[i], NULL);
  }
  //pthread_mutex_destroy(&mutex);

}
