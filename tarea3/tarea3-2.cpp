#include <iostream>
#include <time.h>
#include <vector>
#include <string>
using namespace std;
int id = 0;

class File
{
public:
  int id_inodo;
  string name;
  string date_creation;
  int size;

  File(string name)
  {
    this->id_inodo = id;
    this->name = name;
    time_t fecha = time(0);
    tm *gmtm = gmtime(&fecha);
    this->date_creation = to_string(gmtm->tm_mday) + "/" + to_string(gmtm->tm_mon + 1) + "/" + to_string(gmtm->tm_year + 1900);
    this->size = rand() % 100;
    id++;
  }
};

class Folder
{
public:
  int id_inodo;
  string name;
  Folder *parent;
  string date_creation;
  int size;
  vector<File> files_hijas;
  vector<Folder> carpetas_hijas;

  Folder(string name)
  {
    this->id_inodo = id;
    this->name = name;
    time_t fecha = time(0);
    tm *gmtm = gmtime(&fecha);
    this->date_creation = to_string(gmtm->tm_mday) + "/" + to_string(gmtm->tm_mon + 1) + "/" + to_string(gmtm->tm_year + 1900);
    this->parent = NULL;
    this->size = rand() % 100;
    id++;
  }
  void set_parent(Folder *parent)
  {
    this->parent = parent;
  }
  void mkdir(string nombre)
  {
    Folder nueva_carpeta(nombre);
    nueva_carpeta.set_parent(this);
    carpetas_hijas.push_back(nueva_carpeta);
  }
  void touch(string nombre)
  {
    File nuevo_archivo(nombre);
    files_hijas.push_back(nuevo_archivo);
  }
  bool renombrar(string nombre, string nuevo_nombre)
  {
    for (int i = 0; i < files_hijas.size(); i++)
    {
      if (files_hijas[i].name == nombre)
      {
        files_hijas[i].name = nuevo_nombre;
        return true;
      }
    }
    for (int i = 0; i < carpetas_hijas.size(); i++)
    {
      if (carpetas_hijas[i].name == nombre)
      {
        carpetas_hijas[i].name = nuevo_nombre;
        return true;
      }
    }
    return false;
  }
  void getPath()
  {
    string path;
    if (name != "root")
    {
      path = name;
    }

    Folder *aux = this;
    while (aux->parent != NULL)
    {
      if (aux->parent->name != "root")
      {
        path = aux->parent->name + "/" + path;
        aux = aux->parent;
      }
      else
      {
        aux = aux->parent;
      }
    }
    cout << path << ": ";
  }
  bool eliminar(string nombre)
  {
    // elimina si es archivo
    for (int i = 0; i < files_hijas.size(); i++)
    {
      if (files_hijas[i].name == nombre)
      {
        files_hijas.erase(files_hijas.begin() + i);
        return true;
      }
    }
    // elimina si es carpeta
    for (int i = 0; i < carpetas_hijas.size(); i++)
    {
      if (carpetas_hijas[i].name == nombre)
      {
        carpetas_hijas.erase(carpetas_hijas.begin() + i);
        return true;
      }
    }
    return false;
  }
  bool mv(string nombre, string destino)
  {
    // mover si es archivo
    for (int i = 0; i < files_hijas.size(); i++)
    {
      if (files_hijas[i].name == nombre)
      {
        for (int j = 0; j < carpetas_hijas.size(); j++)
        {
          if (carpetas_hijas[j].name == destino)
          {
            carpetas_hijas[j].files_hijas.push_back(files_hijas[i]);
            files_hijas.erase(files_hijas.begin() + i);
            return true;
          }
        }
        for (int k = 0; k < parent->carpetas_hijas.size(); k++)
        {
          if (parent->carpetas_hijas[k].name == destino)
          {
            parent->carpetas_hijas[k].files_hijas.push_back(files_hijas[i]);
            files_hijas.erase(files_hijas.begin() + i);
            return true;
          }
        }
        if (destino == parent->name)
        {
          parent->files_hijas.push_back(files_hijas[i]);
          files_hijas.erase(files_hijas.begin() + i);
          return true;
        }
      }
    }
    // mover si es carpeta
    for (int i = 0; i < carpetas_hijas.size(); i++)
    {
      if (carpetas_hijas[i].name == nombre)
      {
        for (int j = 0; j < carpetas_hijas.size(); j++)
        {
          if (carpetas_hijas[j].name == destino)
          {
            carpetas_hijas[j].carpetas_hijas.push_back(carpetas_hijas[i]);
            carpetas_hijas.erase(carpetas_hijas.begin() + i);
            return true;
          }
        }
        for (int k = 0; k < parent->carpetas_hijas.size(); k++)
        {
          if (parent->carpetas_hijas[k].name == destino)
          {
            parent->carpetas_hijas[k].carpetas_hijas.push_back(carpetas_hijas[i]);
            carpetas_hijas.erase(carpetas_hijas.begin() + i);
            return true;
          }
        }
        if (destino == parent->name)
        {
          parent->carpetas_hijas.push_back(carpetas_hijas[i]);
          carpetas_hijas.erase(carpetas_hijas.begin() + i);
          return true;
        }
      }
    }
    return false;
  }
  void ls()
  {
    for (int i = 0; i < carpetas_hijas.size(); i++)
    {
      cout << carpetas_hijas[i].name << " ";
    }
    for (int i = 0; i < files_hijas.size(); i++)
    {
      cout << files_hijas[i].name << " ";
    }
    cout << endl;
  }
  void ls_i()
  {
    for (int i = 0; i < carpetas_hijas.size(); i++)
    {
      cout << carpetas_hijas[i].id_inodo << " " << carpetas_hijas[i].name << " ";
    }
    for (int i = 0; i < files_hijas.size(); i++)
    {
      cout << files_hijas[i].id_inodo << " " << files_hijas[i].name << " ";
    }
    cout << endl;
  }

  void ls_R(string path)
  {
    // mostrar archivos y carpetas
    if (path != ".")
    {
      cout << path << ": " << endl;
    }
    this->ls();
    cout << endl;
    for (int i = 0; i < carpetas_hijas.size(); i++)
    {
      carpetas_hijas[i].ls_R(path + "/" + carpetas_hijas[i].name);
    }
  }
  Folder *getFolder(string nombre)
  {
    for (int i = 0; i < carpetas_hijas.size(); i++)
    {
      if (carpetas_hijas[i].name == nombre)
      {
        return &carpetas_hijas[i];
      }
    }
    if (".." == nombre)
    {
      return this->parent;
    }
    return NULL;
  }
};

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
  cout << "Bienvenido al KH-FS" << endl;
  cout << "Estos son los comandos: " << endl;
  // mkdir
  cout << "1. Crear directorio - mkdir" << endl;
  // touch
  cout << "2. Crear archivo - touch" << endl;
  // rn
  cout << "3. Renombrar archivo- rn" << endl;
  // rm
  cout << "4. Eliminar archivo - rm" << endl;
  // mv
  cout << "5. Mover archivo - mv" << endl;
  // ls
  cout << "6. Visualización de archivos - ls" << endl;
  // ls -i
  cout << "7. Visualización de archivos con id - ls -i" << endl;
  // ls -R
  cout << "8. Visualización de archivos recursiva - ls -R" << endl;
  // cd
  cout << "9. Navegar por el sistema de archivos - cd" << endl;
  // exit
  cout << "10. Salir - exit" << endl;

  string nombre;
  cout << "Ingrese nombre de usuario: ";
  getline(cin, nombre);
  Folder root("root");
  Folder *current = &root;

  string opcion;
  cout << nombre << "/";
  current->getPath();
  getline(cin, opcion);
  vector<string> palabras;
  separar_instrucciones(opcion, palabras);

  opcion = palabras[0];

  while (opcion != "exit")
  {
    if (opcion == "mkdir")
    {
      string nombre = palabras[1];
      current->mkdir(nombre);
    }
    else if (opcion == "touch")
    {

      string nombre = palabras[1];
      current->touch(nombre);
    }
    else if (opcion == "rn")
    {

      string nombre = palabras[1];
      string nuevo_nombre = palabras[2];
      if (current->renombrar(nombre, nuevo_nombre))
      {
        cout << "Archivo renombrado" << endl;
      }
      else
      {
        cout << "No se encontro el archivo" << endl;
      }
    }
    else if (opcion == "rm")
    {

      string nombre = palabras[1];
      if (current->eliminar(nombre))
      {
        cout << "Archivo eliminado" << endl;
      }
      else
      {
        cout << "No se encontro el archivo" << endl;
      }
    }
    else if (opcion == "mv")
    {

      string nombre = palabras[1];
      string destino = palabras[2];
      if (current->mv(nombre, destino))
      {
        cout << "Archivo movido" << endl;
      }
      else
      {
        cout << "No se encontro el archivo o carpeta de destino" << endl;
      }
    }
    else if (opcion == "ls")
    {

      current->ls();
    }
    else if (opcion == "ls -i")
    {
      current->ls_i();
    }
    else if (opcion == "cd")
    {

      string nombre = palabras[1];
      if (current->getFolder(nombre) != NULL)
      {
        current = current->getFolder(nombre);
      }
      else
      {
        cout << "No existe la carpeta" << endl;
      }
    }
    else if (opcion == "ls -R")
    {
      current->ls_R(".");
    }
    else
    {
      cout << "Opcion no valida" << endl;
    }
    cout << nombre << "/";
    current->getPath();
    getline(cin, opcion);
    palabras.clear();
    separar_instrucciones(opcion, palabras);
    opcion = palabras[0];
  }
  return 0;
}

/*
comando a ejecutar
mkdir carpeta1
mkdir carpeta2
ls
rn carpeta1 carpeta_nueva
mv carpeta_nueva carpeta2
ls
rm carpeta2
ls
mkdir carpeta1
mkdir carpeta2
cd carpeta1
mkdir carpeta3
cd carpeta3
mkdir carpeta4
touch archivo1
touch archivo2
ls
ls -i
cd ..
cd ..
ls -R
*/