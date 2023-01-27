#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
using namespace std;

class Arista
{
public:
	int nodo1;
	int nodo2;
	int peso;
	Arista(int nodo1, int nodo2, int peso)
	{
		this->nodo1 = nodo1;
		this->nodo2 = nodo2;
		this->peso = peso;
	}
};

vector<Arista> aristas;

int main()
{

	return 0;
}