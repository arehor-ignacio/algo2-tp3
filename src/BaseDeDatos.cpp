#include "BaseDeDatos.h"

const string_map<Tabla>& BaseDeDatos::tablas() const {
    return this->_tablas;
}

const Tabla& BaseDeDatos::obtenerTabla(const NombreTabla& n) const {
    return this->_tablas.at(n);
}

void BaseDeDatos::agregarTabla(const NombreTabla& n, const Tabla& t) {
    this->_tablas[n] = t;
}

void BaseDeDatos::borrarTabla(const NombreTabla& n) {
    this->_tablas.erase(n);
}

void BaseDeDatos::agregarRegistro(const NombreTabla& n, const Registro& r) {
    this->_tablas.at(n).insertar(r);
}

void BaseDeDatos::borrarRegistro(const NombreTabla& n, const Valor& v) {
    this->_tablas.at(n).borrar(v);
};