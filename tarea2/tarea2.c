#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

// int path = 0;

int grafito[12][12] = {
	0 {-1, 3, 2, 4, -1, -1, -1, -1, -1, -1, -1, -1},
	1 {-1, -1, 6, -1, 9, -1, -1, -1, -1, -1, -1, -1},
	2 {-1, -1, -1, 7, -1, 1, -1, -1, -1, -1, -1, -1},
	3 {-1, -1, -1, -1, -1, -1, 9, 3, -1, -1, -1, -1},
	4 {-1, -1, -1, -1, -1, 4, -1, -1, 8, -1, -1, -1},
	5 {-1, -1, -1, -1, -1, -1, 5, -1, -1, 13, -1, -1},
	6 {-1, -1, -1, -1, -1, -1, -1, 17, -1, -1, 10, -1},
	7 {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 15, -1},
	8 {-1, -1, -1, -1, -1, -1, -1, -1, -1, 3, -1, 11},
	9 {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 4},
	10 {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1},
	11 {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
};

// recorrer el grafo y calcular el camino

void get_path(int row, int col, int path)
{
}

int main()
{
	get_path(0, 0, 0);

	return 0;
}