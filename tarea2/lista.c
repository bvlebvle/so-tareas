#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

#define Nodo struct nodo
#define Arista struct arista

Nodo
{
	int id;
	int peso;
	Nodo *next;
	Arista *adyacencia;
};
Arista
{
	Nodo *vrt;
	Arista *next;
};

Nodo *inicio = NULL;