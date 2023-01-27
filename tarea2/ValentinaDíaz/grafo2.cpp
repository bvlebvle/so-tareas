#include <iostream>
#include <vector>
#include <unistd.h>
#include <time.h>
#include <thread>
#include <semaphore.h>
#include <mutex>

using namespace std;

class Nodo
{
public:
	int id;
	int cost;
	int id_arista;

	Nodo(int id, int cost, int id_arista)
	{
		this->id = id;
		this->cost = cost;
		this->id_arista = id_arista;
	}
};

int low_cost = 1000000000;
string low_path = "";
vector<vector<Nodo>> grafo;
sem_t *semaphores[20]; // 1 para cada arista
mutex mtx;

void llenar_grafo()
{
	vector<Nodo> nodo0;
	nodo0.push_back(Nodo(1, 3, 0));
	nodo0.push_back(Nodo(2, 2, 1));
	nodo0.push_back(Nodo(3, 4, 2));
	grafo.push_back(nodo0);
	// nodo 1
	vector<Nodo> nodo1;
	nodo1.push_back(Nodo(2, 6, 3));
	nodo1.push_back(Nodo(4, 9, 4));
	grafo.push_back(nodo1);
	// nodo 2
	vector<Nodo> nodo2;
	nodo2.push_back(Nodo(3, 7, 5));
	nodo2.push_back(Nodo(5, 1, 6));
	grafo.push_back(nodo2);
	// nodo 3
	vector<Nodo> nodo3;
	nodo3.push_back(Nodo(6, 9, 7));
	nodo3.push_back(Nodo(7, 3, 8));
	grafo.push_back(nodo3);
	// nodo 4
	vector<Nodo> nodo4;
	nodo4.push_back(Nodo(5, 4, 9));
	nodo4.push_back(Nodo(8, 8, 10));
	grafo.push_back(nodo4);
	// nodo 5
	vector<Nodo> nodo5;
	nodo5.push_back(Nodo(6, 5, 11));
	nodo5.push_back(Nodo(9, 13, 12));
	grafo.push_back(nodo5);
	// nodo 6
	vector<Nodo> nodo6;
	nodo6.push_back(Nodo(7, 17, 13));
	nodo6.push_back(Nodo(10, 10, 14));
	grafo.push_back(nodo6);
	// nodo 7
	vector<Nodo> nodo7;
	nodo7.push_back(Nodo(10, 15, 15));
	grafo.push_back(nodo7);
	// nodo 8
	vector<Nodo> nodo8;
	nodo8.push_back(Nodo(9, 3, 16));
	nodo8.push_back(Nodo(11, 9, 17));
	grafo.push_back(nodo8);
	// nodo 9
	vector<Nodo> nodo9;
	nodo9.push_back(Nodo(11, 4, 18));
	grafo.push_back(nodo9);
	// nodo 10
	vector<Nodo> nodo10;
	nodo10.push_back(Nodo(11, 1, 19));
	grafo.push_back(nodo10);
	// nodo 11
	vector<Nodo> nodo11;
	nodo11.push_back(Nodo(0, 0, 20));
	grafo.push_back(nodo11);
}

void getCost(int nodo, int cost, string path)
{
	if (nodo != 11)
	{
		int neighbors = grafo[nodo].size();	   // numero de vecinos
		int i = rand() % neighbors;			   // selecciona un vecino al azar
		int nodo_vecino = grafo[nodo][i].id;   // id del vecino
		int arista = grafo[nodo][i].id_arista; // id de la arista

		sem_wait(semaphores[arista]);		  // espera a que el semaforo este libre
		path += to_string(nodo_vecino) + " "; // agrergar id al path
		cost += grafo[nodo][i].cost;
		sem_post(semaphores[arista]);

		getCost(nodo_vecino, cost, path);
	}

	else
	{
		mtx.lock();

		if (cost < low_cost)
		{
			low_cost = cost;
			low_path = path;
		}
		mtx.unlock();
		return;
	}
}
// funcion asosciada al thread
void getCostThread()
{
	getCost(0, 0, "0 ");
}

int main()
{
	// para los valores aleatorios
	srand(time(NULL));
	// funcion que llena el grafo
	llenar_grafo();

	for (int i = 0; i < 20; i++)
	{
		int random = rand() % 5; // numero aleatorio entre 0 y 4
		semaphores[i] = new sem_t;
		semaphores[i] = sem_open("pSem", O_CREAT | O_EXCL, 0644, rand);
	}

	int m = 3;
	thread ruteros[m];
	for (int i = 0; i < m; i++)
	{
		ruteros[i] = thread(getCostThread);
		// ruteros[i].join();
	}
	for (int i = 0; i < m; i++)
	{
		ruteros[i].join();
	}

	cout << "Costo menor: " << low_cost << endl;
	cout << "Ruta: " << low_path << endl;

	return 0;
}