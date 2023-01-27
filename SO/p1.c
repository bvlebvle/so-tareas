/*
  Compilar:
    gcc -o pthreads2 pthreads2.c -lpthread
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define K 3
#define N 10

int n_autos;
volatile int estacionamiento[K][N]; // Variable compartida global
pthread_mutex_t locks[K][N];
sem_t * entrada;

int piso()
{
  int i = 0, j = 0;
  for(i = 0; i < K; i++)
  {
    for(j = 0; j < N; j++)
    {
      if(estacionamiento[i][j] == 0)
      {
        return(i);
      }
    }
  }
}

int ubicacion()
{
  int i = 0, j = 0;
  for(i = 0; i < K; i++)
  {
    for(j = 0; j < N; j++)
    {
      if(estacionamiento[i][j] == 0)
      {
        return(j);
      }
    }
  }
}

void *funcionAuto(void *arg)
{
    int piso_esc, ubicacion_esc;
    sem_wait(entrada);
    printf("\nEntro al estacionamiento\n");
    piso_esc = piso();
    ubicacion_esc = ubicacion();

    //critical section
    pthread_mutex_lock(&(locks[piso_esc][ubicacion_esc]));
    estacionamiento[piso_esc][ubicacion_esc] = 1;    
    sleep(5);
    estacionamiento[piso_esc][ubicacion_esc] = 0;
    pthread_mutex_unlock(&(locks[piso_esc][ubicacion_esc]));
     
    //signal
    printf("\nSalgo del estacionamiento\n");
    sem_post(entrada);
    return NULL;
}
                                                                             
int main(int argc, char *argv[])
{                   
    if (argc != 2) 
    {
      fprintf(stderr, "uso: ./p1 <n_autos>\n");
      exit(1);
    }
    n_autos = atoi(argv[1]);

    int i = 0, j = 0; 
    for(i = 0; i < K; i++)
    {
      for(j = 0; j < N; j++)
      {
        estacionamiento[i][j] = 0;
        pthread_mutex_init(&(locks[i][j]), NULL);
      }
    }

    entrada = sem_open("entrada", O_CREAT | O_EXCL, 0644, N*K); 
    pthread_t autos[n_autos];
    for(i = 0; i < n_autos; i++)
    {
      pthread_create(&(autos[i]), NULL, funcionAuto, NULL);
    }
    
    for(i = 0; i < n_autos; i++)
    {
      pthread_join(autos[i], NULL);
    }
    
    for(i = 0; i < K; i++)
    {
      for(j = 0; j < N; j++)
      {
        pthread_mutex_destroy(&(locks[i][j]));
      }
    }
    sem_close(entrada);
    sem_unlink("entrada");
    return 0;
}

