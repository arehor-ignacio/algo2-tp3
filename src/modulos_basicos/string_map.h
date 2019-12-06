#ifndef STRING_MAP_H_
#define STRING_MAP_H_

#define SIGUIENTES_SIZE 256

#include <string>
#include <set>
#include "linear_set_iterators.h"

using namespace std;

namespace algo2 {

template<typename T>
class string_map{
public:
    /**
    CONSTRUCTOR
    * Construye un diccionario vacio.
    **/
    string_map();

    /**
    CONSTRUCTOR POR COPIA
    * Construye un diccionario por copia.
    **/
    string_map(const string_map<T> &aCopiar);

    /**
    OPERADOR ASIGNACION
     */
    string_map &operator=(const string_map &d);


    bool operator==(const string_map&) const;

    /**
    DESTRUCTOR
    **/
    ~string_map();

    /**
    INSERT
    * Inserta un par clave, valor en el diccionario
    **/
    void insert(const pair<string, T> &);

    /**
    COUNT
    * Devuelve la cantidad de apariciones de la clave (0 o 1).
    * Sirve para identificar si una clave está definida o no.
    **/

    int count(const string &key) const;

    /**
    AT
    * Dada una clave, devuelve su significado.
    * PRE: La clave está definida.
    --PRODUCE ALIASING--
    -- Versión modificable y no modificable
    **/
    const T &at(const string &key) const;

    T &at(const string &key);

    /**
    ERASE
    * Dada una clave, la borra del diccionario junto a su significado.
    * PRE: La clave está definida.
    --PRODUCE ALIASING--
    **/
    void erase(const string &key);

    /**
     SIZE
     * Devuelve cantidad de claves definidas */
    int size() const;

    /**
     EMPTY
     * devuelve true si no hay ningún elemento en el diccionario */
    bool empty() const;

    /** OPTATIVO
    * operator[]
    * Acceso o definición de pares clave/valor
    **/
    T &operator[](const string &key);

    const std::set<string>& keys() const;

private:

    struct Nodo {
        Nodo *siguientes[256];
        T *definicion;
        bool esFinDePalabra;
        set<string>::iterator itClave;

        Nodo() : definicion(new T()), esFinDePalabra(false){
            for (int i = 0; i < 256; ++i) {
                siguientes[i] = nullptr;
            }
        };

        bool esHoja() {
            int i = 0;
            while (i < SIGUIENTES_SIZE && this->siguientes[i] == nullptr) ++i;
            return i == SIGUIENTES_SIZE;
        }
    };

    void eliminarNodo(struct Nodo *);

    struct Nodo *removerNodo(struct Nodo *&, const string &, int);

    Nodo *_raiz;
    std::set<string> _keys;
};

}
#include "string_map.hpp"

#endif // STRING_MAP_H_