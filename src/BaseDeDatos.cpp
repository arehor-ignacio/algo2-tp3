#include "BaseDeDatos.h"

BaseDeDatos::BaseDeDatos() {};

const string_map<Tabla>& BaseDeDatos::tablas() const {
    return this->_tablas;
}

/* Pre: Def(n, Tablas) */
const Tabla& BaseDeDatos::obtenerTabla(const NombreTabla& n) const {
    return this->_tablas.at(n);
}

void BaseDeDatos::agregarTabla(const NombreTabla& n, const Tabla& t) {
    this->_tablas[n] = t;
}
/* Pre: Def(n, Tablas) */
void BaseDeDatos::borrarTabla(const NombreTabla& n) {
    this->_tablas.erase(n);
}
/* Pre: Def(n, Tablas) ^ Campos(r) = Campos(Obtener(n, Tablas)) */
void BaseDeDatos::agregarRegistro(const NombreTabla& n, const Registro& r) {
    this->_tablas.at(n).insertar(r);
}
/* Pre: Def(n, Tablas) ^ ClaveDefinida(r, Obtener(n, Tablas)) */
void BaseDeDatos::borrarRegistro(const NombreTabla& n, const Valor& v) {
    this->_tablas.at(n).borrar(v);
};

