Se creó la clase Nodo que tiene como atributo el id del nodo, el costo para llegar a el y el id de la arista
que lo une con su vecino.
Se crearon variables globales:

1. low_cost, que comienza con un valor muy alto que representa el costo de recorrer el grafo
2. low_path, que comienza como un string vacío y representa la ruta que sigue el thread
3. grafo, que es una vector de vectores de nodos, que representa el nodo y sus vecinos.
4. semaphores, que es un arreglo que semaforos de tamaño 20, que es la cantidad de arista, por lo tanto, hay un semaforo para cada arista
5. mtx, que es un mutex

Está la función llenar_grafo que llena el grafo con valores.
Está la funcion getCost que recorre el grafo recursivamente, está función es la principal debido a que cada threads para por aquí para calcular el costo y ruta.
Está la función getCostThread que es la función asociada al thread que llama a la función getCost.

En el main se llama la función para llenar el grafo, se crean los semaforos. A estos se asigna les
un numero aleatorio entre 0 y 4 (se puede modificar si quieres, int random = rand() % 5, se debe cambiar el 5 por el numero que quieras)
de thread maximos por arista. También, se crean los "m" threads (también se puede cambiar si lo necesitas) a través de un for y se asocian a
la función getCostThread, después en otro for se hace join a los threads. Y para terminar se imprime por pantalla low_cost y low_path.

Me gustaría explicar mejor la función getCost porque es la protagonista del programa. La función recibe 3 parametros:
nodo, cost y path. Nodo hace referencia en el nodo que estoy, cost representa el costo que lleva acumulado para llegar a ese nodo y path es la ruta,
que vendría siendo los nodos por los cuales ha pasado.
Primero se corrobora que no esté en el ultimo nodo, si no está en el último, se calcula el número
de vecinos que tiene el nodo, esto se hace con el tamaño del vector de vecinos asociado a ese nodo, para generar la ruta aleatoria se escoge un número entre 0 y la cantidad de vecinos

-   1, además, se crea una variable que almacena el id del nodo siguiente, también se guarda la arista que une a ambos nodos para posteriormente activar el semaforo asociado a esa arista.
    Cuando se logra pasar por la arista, se agrega a path el id del nodo y se suma a cost el costo asociado al nodo vecino, en ese momento se sale del semaforo. Para finalizar,
    se llama a la función getCost pero con el id del nodo vecino, el costo acumulado a este momento y la ruta actualizada.
    Cuando se llega al último nodo, se utiliza un mutex asegurar que se esté trabajando con el low_cost de ese momento, y se hace una comparación. Si
    cost es menor a low_cost se cambiar el valor de low_cost por cost y low_path queda con el valor de path. Se libera el mutex y se termina la función.

Para compilar y ejecutar se debe usar el siguiente comando:
g++ grafo2.cpp -o grafo -std=c++20 && ./grafo
