#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

int counter=0;
sem_t semaforo, s2;
pthread_t thread[2];

void drink(){
  while (1){
    
    sem_wait(&semaforo);
    counter++;

    if(counter == 1){
      sem_wait(&s2);
    }
    sem_post(&semaforo);
    printf("brebaje\n");
    sem_wait(&semaforo);
    counter--;

    if(counter == 0){
      sem_post(&s2);
    }
    sem_post(&semaforo);

  }
    
}
void eat(){
  while (1)
  {
   sem_wait(&s2);
    printf("buena comida \n");
    sem_post(&s2);
  }
  
    
}
  
void *fthread(){
    eat();
    drink();
}
int main(){
  sem_init(&semaforo, 0,0);
  sem_init(&s2, 0,0);
  
  pthread_create(&thread[0], NULL, &drink, NULL);
  pthread_create(&thread[1], NULL, &eat, NULL);
  
  for (int i=0; i < 2; i++){
    pthread_join(thread[i],NULL);
  }

  sem_destroy(&semaforo);
  sem_destroy(&s2);

  return 0;

}