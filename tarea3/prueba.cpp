#include <iostream>
#include <vector>
#include <string>
using namespace std;
void separar_instrucciones(string opcion, vector<string> &palabras)
{
	opcion = opcion + " ";
	int contador = 0;
	string instrucciones, nombre, destino;
	if (opcion[3] == '-')
	{
		instrucciones = opcion.substr(0, 5);
		palabras.push_back(instrucciones);
	}
	else
	{
		for (int i = 0; i < opcion.size(); i++)
		{
			if (opcion[i] == ' ')
			{
				instrucciones = opcion.substr(contador, i - contador);
				contador = i + 1;
				palabras.push_back(instrucciones);
				instrucciones = "";
			}
		}
	}
}
int main()
{
	vector<string> palabras;
	separar_instrucciones("ls -R", palabras);
	for (int i = 0; i < palabras.size(); i++)
	{
		cout << palabras[i] << endl;
	}

	return 0;
}