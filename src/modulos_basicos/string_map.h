#ifndef STRING_MAP_H_
#define STRING_MAP_H_

#include <string>
#include <stack>
#include <list>
#include <vector>
#include "linear_set.h"
#include "linear_set_iterators.h"

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
    operator==
    * Devuelve true sii las keys son iguales y todos sus valores son iguales
    **/
    bool operator==(const string_map toCompare) const;

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
    const linear_set<string>& keys() const;

private:

    struct Nodo {
        vector<Nodo *> _next;
        pair<T *, linear_set<string>::iterator> _def;

        //T* _def;
        //linear_set<string>::iterator _itClave;

        Nodo() : _next(256, nullptr), _def(nullptr, _template.begin()) {};

        //Nodo() : _next(256, nullptr), _def(nullptr) {};

        Nodo(T *val) : _next(256, nullptr), _def(val, _template.begin()) {

        };

        Nodo(T *val, linear_set<string>::iterator it) : _next(256, nullptr), _def(val, it) {};
    };

    void _createRoot();

    bool _hasChild(Nodo *node);

    void _destroy(Nodo *node);

    void _reCreate(Nodo *node, Nodo *nodeToCopy);

    void _assignIterators();

    //This should be defined in linear_set<T>::iterator, but it isn't. We need it get the last it
    //Because of this, the complexity became linear instead of constant. If it was declared in linear_set it would be constant
    // linear_set<string>::iterator _getLastIt(linear_set<string>::iterator begin, linear_set<string>::iterator last);

    Nodo *_root;
    int _size;
    linear_set<string> _keys;
    static linear_set<string> _template;
};

#include "string_map.hpp"

#endif // STRING_MAP_H_