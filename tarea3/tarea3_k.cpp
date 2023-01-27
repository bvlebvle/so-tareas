#include <iostream>
#include <time.h>
#include <vector>
using namespace std;
int id = 0;
class File
{
public:
	int id_inodo;
	string name;
	string date_creation;
	bool is_folder;
	int size;
	File *parent;
	vector<File> files;

	File(string name, bool is_folder)
	{
		this->id_inodo = id;
		this->name = name;
		time_t fecha = time(0);
		tm *gmtm = gmtime(&fecha);
		this->date_creation = to_string(gmtm->tm_mday) + "/" + to_string(gmtm->tm_mon + 1) + "/" + to_string(gmtm->tm_year + 1900);
		this->size = rand() % 100;
		this->is_folder = is_folder;
		id++;
	}
};

class Directory
{
public:
	File *root;
	File *current;
	Directory()
	{
		this->root = File("root", true);
		this->current = this->root;
	}
	void create_file(string name)
	{
		File file = File(name, false);
		file.parent = &root;
		this->root.files.push_back(file);
	}
	void create_folder(string name)
	{
		File folder = File(name, true);
		folder.parent = &root;
		this->root.files.push_back(folder);
	}
};
int main()
{

	Directory directory = Directory();

	return 0;
}