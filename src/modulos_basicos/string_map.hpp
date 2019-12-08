#include "string_map.h"

//This is used to initialize Nodo, as we dont have an empty init for linear_set iterator
template<typename T>
linear_set<string> string_map<T>::_template = linear_set<string>();

template<typename T>
string_map<T>::string_map() {
    _root = nullptr;
    _size = 0;
    _keys = linear_set<string>();
};

template<typename T>
string_map<T>::string_map(const string_map<T> &toCopy)
        : string_map() { *this = toCopy; }

template<typename T>
string_map<T> &string_map<T>::operator=(const string_map<T> &d) {
    _destroy(_root);

    if (d._root != nullptr) {
        _root = new Nodo(new T(*(d._root->_def.first)));
        _size = d._size;

        _reCreate(_root, d._root);
    }

    _keys = d.keys();

    _assignIterators();

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

        actual->_def.first = new T();
        actual->_def.second = _keys.fast_insert(key);
        _size++;
        // actual->_def.second = _getLastIt(_keys.begin(), _keys.end());

        return *(actual->_def.first);
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

    return actual->_def.first != nullptr;
}

template<typename T>
const T &string_map<T>::at(const string &key) const {
    Nodo *actual = _root;

    for (char c : key) {
        actual = actual->_next[int(c)];
    }

    return *(actual->_def.first);
}

template<typename T>
T &string_map<T>::at(const string &key) {
    Nodo *actual = _root;

    for (char c : key) {
        actual = actual->_next[int(c)];
    }

    return *(actual->_def.first);
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

    delete actual->_def.first;
    actual->_def.first = nullptr;
    _keys.erase(actual->_def.second);

    while (!_hasChild(actual) && !nodeStk.empty()) {
        delete actual->_def.first;
        delete actual;

        actual = nodeStk.top();
        nodeStk.pop();
        actual->_next[keyStk.top()] = nullptr;
        keyStk.pop();
    }

    _size--;
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
        _root->_def.first = new T();
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
        delete node->_def.first;

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
            Nodo *child;
            if (next->_def.first != nullptr) {
                T *definition = new T();
                *definition = *(next->_def.first);
                child = new Nodo(definition);
            } else {
                child = new Nodo();
            }
            node->_next[i] = child;
            _reCreate(child, next);
        }
    }
}

template<typename T>
void string_map<T>::insert(const pair<string, T> &elem) {
    (*this)[elem.first] = elem.second;
}

template<typename T>
const linear_set<string> &string_map<T>::keys() const {
    return _keys;
}

template<typename T>
void string_map<T>::_assignIterators() {
    auto it = _keys.begin();
    while (it != _keys.end()) {
        Nodo *actual = _root;

        for (char c : *it) {
            actual = actual->_next[int(c)];
        }

        actual->_def.second = it;
        ++it;
    }
}

template<typename T>
bool string_map<T>::operator==(const string_map toCompare) const {
    if (!(this->_keys == toCompare._keys)) { return false; }

    if (_keys.size() == 0) { return true; }

    for (string key : this->keys()) {
        bool isKeyEqual = (*this).at(key) == toCompare.at(key);
        if (!isKeyEqual) {
            return false;
        }
    }

    return true;
}

/*
template<typename T>
linear_set<string>::iterator
string_map<T>::_getLastIt(linear_set<string>::iterator begin, linear_set<string>::iterator last) {
    linear_set<string>::iterator prior = begin;
    while(++begin != last) {
        prior = begin;
    }
    return prior;
}
*/