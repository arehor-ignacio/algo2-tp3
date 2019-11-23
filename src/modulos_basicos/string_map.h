#ifndef STRING_MAP_H_
#define STRING_MAP_H_

#include <string>
#include <stack>
#include <list>
#include <vector>

using namespace std;

template<typename T>
class string_map {
public:
    /**
    CONSTRUCTOR
    * Construye un diccionario vacio.
    * Time complexity: O(1)
    **/
    string_map();

    /**
    CONSTRUCTOR POR COPIA
    * Construye un diccionario por copia.
    * * Time complexity: O(toCopy.size)
    **/
    string_map(const string_map<T> &toCopy);

    /**
    OPERADOR ASIGNACION
     */
    string_map &operator=(const string_map &d);

    /**
    DESTRUCTOR
    **/
    ~string_map();

    /**
    INSERT 
    * Inserta un par clave, valor en el diccionario
    * Time complexity: O( (elem.first).size * copy(elem.second) )
    **/
    void insert(const pair<string, T> &elem);

    /**
    operator[]
    * Acceso o definición de pares clave/valor
    * Time complexity: O(key.size)
    **/

    T &operator[](const string &key);

    /**
    COUNT
    * Devuelve la cantidad de apariciones de la clave (0 o 1).
    * Sirve para identificar si una clave está definida o no.
    * Time complexity: O(key.size)
    **/

    int count(const string &key) const;

    /**
    AT
    * Dada una clave, devuelve su significado.
    * PRE: La clave está definida.
    --PRODUCE ALIASING--
    -- Versión modificable y no modificable
    * Time complexity: O(key.size)
    **/
    const T &at(const string &key) const;

    T &at(const string &key);

    /**
    ERASE
    * Dada una clave, la borra del diccionario junto a su significado.
    * PRE: La clave está definida.
    --PRODUCE ALIASING--
    * Time complexity: O(key.size)
    **/
    void erase(const string &key);

    /**
     SIZE
     * Devuelve cantidad de claves definidas
     * * Time complexity: O(1)
     **/
    int size() const;

    /**
     EMPTY
     * devuelve true si no hay ningún elemento en el diccionario
     * * Time complexity: O(1)
     * */
    bool empty() const;

    /**
     * KEYS
     * returns a set<string> representing every key added to the map
     * Time complexity: O(1)
     * */
    const list<string>& keys() const;

private:

    struct Nodo {
        vector<Nodo *> _next;
        pair<T *, list<string>::iterator> _def;

        Nodo() : _next(256, nullptr), _def() {
            _def.first = nullptr;
        };

        Nodo(T *val) : _next(256, nullptr), _def() {
            _def.first = val;
        };

        Nodo(T *val, list<string>::iterator it) : _next(256, nullptr), _def(val, it) {};
    };

    void _createRoot();

    bool _hasChild(Nodo *node);

    void _destroy(Nodo *node);

    void _reCreate(Nodo *node, Nodo *nodeToCopy);

    void _assignIterators();

    Nodo *_root;
    int _size;
    list<string> _keys;
};

#include "string_map.hpp"

#endif // STRING_MAP_H_
