template<typename T>
string_map<T>::string_map() {
    _raiz = nullptr;
    _size = 0;
};

template<typename T>
string_map<T>::string_map(const string_map<T> &aCopiar)
        : string_map() { *this = aCopiar; } // Provisto por la catedra: utiliza el operador asignacion para realizar la copia.

template<typename T>
string_map<T> &string_map<T>::operator=(const string_map<T> &d) {
    _destroy(_raiz);

    if (d._raiz != nullptr) {
        _raiz = new Nodo(new T(*(d._raiz->_definicion)));
        _size = d._size;

        _reCreate(_raiz, d._raiz);
    }

    return *this;
}

template<typename T>
string_map<T>::~string_map() {
    _destroy(_raiz);
    delete _raiz;
}

template<typename T>
T &string_map<T>::operator[](const string &clave) {
    _createRoot();

    //Checkeo si esta definido
    if (count(clave) == 0) {
        //Si lo esta creo la rama para la clave
        Nodo *actual = _raiz;

        for (char c : clave) {
            //Veo si el sig es null
            if (actual->_siguientes[c] == nullptr) {
                //Si lo es, le creo un nodo a ese caracter
                Nodo *missing = new Nodo();
                actual->_siguientes[c] = missing;
            }
            //Avanzo en el string
            actual = actual->_siguientes[c];
        }

        actual->_definicion = new T();

        _size++;

        return *(actual->_definicion);
    } else {
        //Sino lo busco normal
        return at(clave);
    }
}

template<typename T>
int string_map<T>::count(const string &clave) const {
    Nodo *actual = _raiz;

    if (actual == nullptr) {
        return 0;
    }

    for (char c : clave) {
        if (actual->_siguientes[c] == nullptr) {
            return 0;
        }
        actual = actual->_siguientes[c];
    }

    return actual -> _definicion != nullptr;
}

template<typename T>
const T &string_map<T>::at(const string &clave) const {
    at(clave);
}

template<typename T>
T &string_map<T>::at(const string &clave) {
    Nodo *actual = _raiz;

    for (char c : clave) {
        actual = actual->_siguientes[int(c)];
    }

    return *(actual->_definicion);
}

template<typename T>
void string_map<T>::erase(const string &clave) {
    Nodo *actual = _raiz;
    stack<Nodo *> nodeStk;
    stack<int> keyStk;

    for (char c : clave) {
        nodeStk.push(actual);
        keyStk.push(int(c));
        actual = actual->_siguientes[int(c)];
    }

    delete actual->_definicion;
    actual->_definicion = nullptr;

    while (!_hasChild(actual) && !nodeStk.empty()) {
        delete actual->_definicion;
        delete actual;

        actual = nodeStk.top();
        nodeStk.pop();
        actual -> _siguientes[keyStk.top()] = nullptr;
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
typename string_map<T>::Nodo *string_map<T>::_createRoot() {
    if (size() == 0) {
        _raiz = new string_map<T>::Nodo();
        _raiz->_definicion = new T();
    }
}

template<typename T>
bool string_map<T>::_hasChild(string_map::Nodo *node) {
    for (Nodo *aux : node->_siguientes) {
        if (aux != nullptr) {
            return true;
        }
    }
    return false;
}

template<typename T>
void string_map<T>::_destroy(Nodo *node) {
    if (node != nullptr) {
        delete node->_definicion;

        for (Nodo *next : node->_siguientes) {
            if (next != nullptr) {
                _destroy(next);
                delete next;
            }
        }
    }
}

template<typename T>
void string_map<T>::_reCreate(Nodo *node, Nodo *nodeToCopy) {
    for (int i = 0; i < nodeToCopy->_siguientes.size(); i++) {
        Nodo *next = nodeToCopy->_siguientes[i];
        if (next != nullptr) {
            Nodo* child;
            if (next -> _definicion != nullptr){
                T* definition = new T();
                *definition = *(next -> _definicion);
                child = new Nodo(definition);
            } else {
                child = new Nodo();
            }
            node -> _siguientes[i] = child;
            _reCreate(child, next);
        }
    }
}

template<typename T>
void string_map<T>::insert(const pair<string, T> &elem) {
    (*this)[elem.first] = elem.second;
}

