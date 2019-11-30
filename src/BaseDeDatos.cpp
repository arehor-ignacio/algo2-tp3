#include "BaseDeDatos.h"

const linear_set<NombreTabla>& BaseDeDatos::tablas() const {
    return _tables.keys();
}

const Tabla &BaseDeDatos::obtenerTabla(const NombreTabla &tableName) const {
    return _tables.at(tableName);
}

void BaseDeDatos::agregarTabla(const NombreTabla &tableName, Tabla table) {
    _tables[tableName] = table;
}

void BaseDeDatos::borrarTabla(const NombreTabla &tableName) {
    _tables.erase(tableName);
}

void BaseDeDatos::agregarRegistro(const NombreTabla &tableName, Registro reg) {
    _tables[tableName].insertar(reg);
}

void BaseDeDatos::borrarRegistro(const NombreTabla &tableName, Registro reg) {
    NombreCampo keyField = _tables[tableName].clave();
    _tables[tableName].borrar(reg[keyField]);
}
