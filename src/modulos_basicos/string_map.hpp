#include "string_map.h"

template<typename T>
string_map<T>::string_map() {
    _root = nullptr;
    _size = 0;
    _keys = set<string>();
};

template<typename T>
string_map<T>::string_map(const string_map<T> &toCopy)
        : string_map() { *this = toCopy; }

template<typename T>
string_map<T> &string_map<T>::operator=(const string_map<T> &d) {
    _destroy(_root);

    if (d._root != nullptr) {
        _root = new Nodo(new T(*(d._root->_def)));
        _size = d._size;

        _reCreate(_root, d._root);
    }

    _keys = d.keys();

    return *this;
}

template<typename T>
string_map<T>::~string_map() {
    _destroy(_root);
    delete _root;
}

template<typename T>
T &string_map<T>::operator[](const string &key) {
    _createRoot();

    //Checkeo si esta definido
    if (count(key) == 0) {
        //Si lo esta creo la rama para la clave
        Nodo *actual = _root;

        for (char c : key) {
            //Veo si el sig es null
            if (actual->_next[c] == nullptr) {
                //Si lo es, le creo un nodo a ese caracter
                Nodo *missing = new Nodo();
                actual->_next[c] = missing;
            }
            //Avanzo en el string
            actual = actual->_next[c];
        }

        actual->_def = new T();

        _size++;
        _keys.insert(key);

        return *(actual->_def);
    } else {
        //Sino lo busco normal
        return at(key);
    }
}

template<typename T>
int string_map<T>::count(const string &key) const {
    Nodo *actual = _root;

    if (actual == nullptr) {
        return 0;
    }

    for (char c : key) {
        if (actual->_next[c] == nullptr) {
            return 0;
        }
        actual = actual->_next[c];
    }

    return actual -> _def != nullptr;
}

template<typename T>
const T &string_map<T>::at(const string &key) const {
    at(key);
}

template<typename T>
T &string_map<T>::at(const string &key) {
    Nodo *actual = _root;

    for (char c : key) {
        actual = actual->_next[int(c)];
    }

    return *(actual->_def);
}

template<typename T>
void string_map<T>::erase(const string &key) {
    Nodo *actual = _root;
    stack<Nodo *> nodeStk;
    stack<int> keyStk;

    for (char c : key) {
        nodeStk.push(actual);
        keyStk.push(int(c));
        actual = actual->_next[int(c)];
    }

    delete actual->_def;
    actual->_def = nullptr;

    while (!_hasChild(actual) && !nodeStk.empty()) {
        delete actual->_def;
        delete actual;

        actual = nodeStk.top();
        nodeStk.pop();
        actual -> _next[keyStk.top()] = nullptr;
        keyStk.pop();
    }

    _size--;
    _keys.erase(key);
}

template<typename T>
int string_map<T>::size() const {
    return _size;
}

template<typename T>
bool string_map<T>::empty() const {
    return size() == 0;
}

template<typename T>
void string_map<T>::_createRoot() {
    if (size() == 0) {
        _root = new string_map<T>::Nodo();
        _root->_def = new T();
    }
}

template<typename T>
bool string_map<T>::_hasChild(string_map::Nodo *node) {
    for (Nodo *aux : node->_next) {
        if (aux != nullptr) {
            return true;
        }
    }
    return false;
}

template<typename T>
void string_map<T>::_destroy(Nodo *node) {
    if (node != nullptr) {
        delete node->_def;

        for (Nodo *next : node->_next) {
            if (next != nullptr) {
                _destroy(next);
                delete next;
            }
        }
    }
}

template<typename T>
void string_map<T>::_reCreate(Nodo *node, Nodo *nodeToCopy) {
    for (int i = 0; i < nodeToCopy->_next.size(); i++) {
        Nodo *next = nodeToCopy->_next[i];
        if (next != nullptr) {
            Nodo* child;
            if (next -> _def != nullptr){
                T* definition = new T();
                *definition = *(next -> _def);
                child = new Nodo(definition);
            } else {
                child = new Nodo();
            }
            node -> _next[i] = child;
            _reCreate(child, next);
        }
    }
}

template<typename T>
void string_map<T>::insert(const pair<string, T> &elem) {
    _keys.insert(elem.first);
    (*this)[elem.first] = elem.second;
}

template<typename T>
set<string> string_map<T>::keys() const{
    return _keys;
}

