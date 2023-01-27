/*
  Compilar:
    gcc -o mutex_bien mutex_bien.c -lpthread
*/
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

//arreglo de threads
pthread_t tid[2];
// contador
int counter;
//mutex
pthread_mutex_t lock;


void* funcionThread(void *arg)
{
    //cierra el candado
    pthread_mutex_lock(&lock);
    unsigned long i = 0;
    counter += 1;
    printf("\n Inicia job %d \n", counter);
    for(i=0; i<(0xFFFFFFF);i++);
    printf("\n Termina job %d \n", counter);
    pthread_mutex_unlock(&lock);
    //abre candado
    // si no se abre el candado no se puede seguir con el siguiente thread
    return NULL;
}
int main(void)
{
    int i = 0;
    int err;

    //creacion de mutex 
    //0 exito, otro valor no se puede crear
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n Falla inicializacion de mutex \n");
        return 1;
    }
    // creacion de threads
    while(i < 2)
    {
        err = pthread_create(&(tid[i]), NULL, &funcionThread, NULL);
        if (err != 0)
            printf("\nNo se puede crear el thread :[%s]", strerror(err));
        i++;
    }
    //ayuda a empezar y terminar
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    //finalizacion de mutex
    pthread_mutex_destroy(&lock);
    return 0;
}