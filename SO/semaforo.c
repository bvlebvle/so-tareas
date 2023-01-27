#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

volatile int counter = 1;
pthread_mutex_t mutex;
sem_t semaforo;

void *t1()
{
  //pthread_mutex_lock(&mutex);

  sem_wait(&semaforo);
  printf("soy un thread\n");
  printf("sigo aqui \n");
  sem_post(&semaforo);

  //pthread_mutex_unlock(&mutex);
  pthread_exit(NULL); //retorna algo
}

int main()
{

  int large = 4;
  pthread_mutex_init(&mutex, NULL);

  //sem_init(&semaforo, 0 , 1);
  semaforo = sem_open("pSem", O_CREAT | O_EXCL, 0644, 1);
  //semaforo
  //0 para que solo pueda ser usado por threads del mismo proceso
  //5 valor inicial del semaforo

  pthread_t thread[large];

  for (long i = 0; i < large; i++)
  {

    pthread_create(&thread[i], NULL, t1, NULL);
  }
  for (int i = 0; i < large; i++)
  {
    pthread_join(thread[i], NULL);
  }
  while(1);

  sem_close(&semaforo);
  sem_unlink("pSem");
  //sem_destroy(&semaforo);
  //pthread_mutex_destroy(&mutex);

}
