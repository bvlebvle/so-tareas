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
  void print_file()
  {
    vector<string> data;
    data.push_back(to_string(this->id_inodo));
    data.push_back(this->name);
    data.push_back(this->date_creation);
    data.push_back(to_string(this->size));
    for (int i = 0; i < data.size(); i++)
    {
      cout << data[i] << "         ";
    }
    cout << endl;
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
  void crear_carpeta(string nombre)
  {
    Folder nueva_carpeta(nombre);
    nueva_carpeta.set_parent(this);
    carpetas_hijas.push_back(nueva_carpeta);
  }
  void crear_archivo(string nombre)
  {
    File nuevo_archivo(nombre);
    files_hijas.push_back(nuevo_archivo);
  }
  bool renombrar_archivo(string nombre, string nuevo_nombre)
  {
    for (int i = 0; i < files_hijas.size(); i++)
    {
      if (files_hijas[i].name == nombre)
      {
        files_hijas[i].name = nuevo_nombre;
        return true;
      }
    }
    return false;
  }
  bool renombrar_carpeta(string nombre, string nuevo_nombre)
  {
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
  bool eliminar_archivo(string nombre)
  {
    for (int i = 0; i < files_hijas.size(); i++)
    {
      if (files_hijas[i].name == nombre)
      {
        files_hijas.erase(files_hijas.begin() + i);
        return true;
      }
    }
    return false;
  }
  bool eliminar_carpeta(string nombre)
  {
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
  bool mover_archivo(string nombre, string destino)
  {
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
    return false;
  }
  bool mover_carpeta(string nombre, string destino)
  {
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
  void visualizacion_arbol()
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
  void print_folder()
  {
    vector<string> data;
    data.push_back(to_string(this->id_inodo));
    data.push_back(this->name);
    data.push_back(this->date_creation);
    data.push_back(to_string(this->size));
    for (int i = 0; i < data.size(); i++)
    {
      cout << data[i] << "         ";
    }
    cout << endl;
  }
  void visualizacion_arbol_detalles()
  {
    cout << "ID   | Nombre   |Fecha de creacion|Tamaño          " << endl;
    for (int i = 0; i < carpetas_hijas.size(); i++)
    {
      carpetas_hijas[i].print_folder();
    }
    for (int i = 0; i < files_hijas.size(); i++)
    {
      files_hijas[i].print_file();
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
    if (this->parent->name == nombre)
    {
      return this->parent;
    }
    return NULL;
  }
};

int main()
{
  cout << "Bienvenido al KH-FS" << endl;
  cout << "Estos son los comandos: " << endl;
  cout << "1. Crear carpeta - cc" << endl;
  cout << "2. Crear archivo - ca" << endl;
  cout << "3. Renombrar archivo - ra" << endl;
  cout << "4. Renombrar carpeta - rc" << endl;
  cout << "5. Eliminar archivo - ea" << endl;
  cout << "6. Eliminar carpeta - ec" << endl;
  cout << "7. Mover archivo - ma" << endl;
  cout << "8. Mover carpeta - mc" << endl;
  cout << "9. Visualización de archivos - va" << endl;
  cout << "10. Visualización de archivos con detalles - vda" << endl;
  cout << "11. Obtener ruta - or" << endl;
  cout << "12. Navegar por el sistema de archivos - nsa" << endl;
  cout << "13. Salir - s" << endl;

  string nombre;
  cout << "Ingrese nombre de usuario: ";
  cin >> nombre;
  Folder root("root");
  Folder *current = &root;

  string opcion;
  cout << nombre << "/";
  current->getPath();
  cin >> opcion;

  while (opcion != "s")
  {
    if (opcion == "cc")
    {
      string nombre;
      cin >> nombre;
      current->crear_carpeta(nombre);
    }
    else if (opcion == "ca")
    {

      string nombre;
      cin >> nombre;
      current->crear_archivo(nombre);
    }
    else if (opcion == "ra")
    {

      string nombre;
      string nuevo_nombre;
      cin >> nombre;
      cin >> nuevo_nombre;
      if (current->renombrar_archivo(nombre, nuevo_nombre))
      {
        cout << "Archivo renombrado" << endl;
      }
      else
      {
        cout << "No se encontro el archivo" << endl;
      }
    }
    else if (opcion == "rc")
    {

      string nombre;
      string nuevo_nombre;
      cin >> nombre;
      cin >> nuevo_nombre;
      if (current->renombrar_carpeta(nombre, nuevo_nombre))
      {
        cout << "Carpeta renombrada" << endl;
      }
      else
      {
        cout << "No se encontro la carpeta" << endl;
      }
    }
    else if (opcion == "ea")
    {

      string nombre;
      cin >> nombre;
      if (current->eliminar_archivo(nombre))
      {
        cout << "Archivo eliminado" << endl;
      }
      else
      {
        cout << "No se encontro el archivo" << endl;
      }
    }
    else if (opcion == "ec")
    {

      string nombre;
      cin >> nombre;
      if (current->eliminar_carpeta(nombre))
      {
        cout << "Carpeta eliminada" << endl;
      }
      else
      {
        cout << "No se encontro la carpeta" << endl;
      }
    }
    else if (opcion == "ma")
    {

      string nombre;
      string destino;
      cin >> nombre;
      cin >> destino;
      if (current->mover_archivo(nombre, destino))
      {
        cout << "Archivo movido" << endl;
      }
      else
      {
        cout << "No se encontro el archivo o carpeta de destino" << endl;
      }
    }
    else if (opcion == "mc")
    {

      string nombre;
      string destino;
      cin >> nombre;
      cin >> destino;
      if (current->mover_carpeta(nombre, destino))
      {
        cout << "Carpeta movida" << endl;
      }
      else
      {
        cout << "No se encontro la carpeta o carpeta de destino" << endl;
      }
    }
    else if (opcion == "va")
    {

      current->visualizacion_arbol();
    }
    else if (opcion == "or")
    {
      cout << nombre << "/";
      current->getPath();
    }
    else if (opcion == "nsa")
    {

      string nombre;
      cin >> nombre;
      if (current->getFolder(nombre) != NULL)
      {
        current = current->getFolder(nombre);
      }
      else
      {
        cout << "No existe la carpeta" << endl;
      }
    }
    else if (opcion == "vad")
    {
      current->visualizacion_arbol_detalles();
    }
    else
    {
      cout << "Opcion no valida" << endl;
    }
    cout << nombre << "/";
    current->getPath();
    cin >> opcion;
  }
  return 0;
}

/*
comando a ejecutar
cc c1
ca a1
ra a1 archivo
ea archivo
rc c1 carpeta
ec carpeta
va
cc c1
nsa c1
cc c2
nsa c2
cc c3
cc c4
ca a1
va
ma a1 c3
va
mc c3 c4
va
*/