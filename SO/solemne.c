
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


#define k 1
#define n 2

volatile int estacionamiento[k][n];
pthread_mutex_t locks[k][n];
//pthread_mutex_t entrada;
sem_t entrada;
int libres = n * k; 



// obtener piso
int get_piso(){

    for(int i=0; i< k; i++){
      for(int j=0; j< n; j++){
        if (estacionamiento[i][j]==0){
            return i;
        }
      }
    }
   // return -1; 
}

// obtener puesto
int get_puesto(){
    for(int i=0; i< k; i++){
      for(int j=0; j< n; j++){
        if (estacionamiento[i][j]==0){
            return j;
        }
      }
    }
   // return -1; 
}


void *parking(){

  sem_wait(&entrada);
  printf("Entrando al estacionamiento\n");
  int piso = get_piso();
  int puesto = get_puesto();

  printf("Estacionamientos libres: %d \n", libres);
  // mapa de cosas libres 
  for(int i=0; i< k; i++){
    printf("Piso: %d \n", i);
    
    for(int j=0; j< n; j++){
      if (estacionamiento[i][j]==0){
        printf("%d, %d \n",i, j);
      }
    }
       printf("\n");
  }

  pthread_mutex_lock(&(locks[piso][puesto]));
  printf("Ocupa estacionamiento %d, %d\n", piso, puesto);
  estacionamiento[piso][puesto]=1;
  libres--;
  sleep(5);
  printf("Libera estacionamiento %d, %d\n", piso, puesto);
  estacionamiento[piso][puesto]=0;
  libres++;
  pthread_mutex_unlock(&(locks[piso][puesto]));
  
  //pthread_mutex_unlock(&entrada);
  sem_post(&entrada);
  return NULL;
}


int main (){
  int n_cars=4; 
  pthread_t cars[n_cars];
  for(int i=0; i< k; i++){
      for(int j=0; j< n; j++){
        estacionamiento[i][j]=0;
        pthread_mutex_init(&(locks[i][j]), NULL);
      }
  }
  entrada = sem_open("entrada", O_CREAT | O_EXCL, 0644, n*k);
   pthread_mutex_init(&entrada, NULL);

  for (long i = 0; i < n_cars; i++)
  {
    pthread_create(&cars[i], NULL, parking, NULL);
  }
  for (int i = 0; i < n_cars; i++)
  {
    pthread_join(cars[i], NULL);
  }

  for(int i=0; i< k; i++){
      for(int j=0; j< n; j++){
        pthread_mutex_destroy(&(locks[i][j]));
      }
  }
  sem_close(&entrada);
  sem_unlink("entrada");
  pthread_mutex_destroy(&entrada);
  return 0;
}
