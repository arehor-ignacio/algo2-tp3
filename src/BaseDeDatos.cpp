#include "BaseDeDatos.h"

BaseDeDatos::BaseDeDatos() {

}

const list<NombreTabla>& BaseDeDatos::Tablas() const {
    return _tables.keys();
}

Tabla &BaseDeDatos::ObtenerTabla(const NombreTabla& tableName) {
    return _tables[tableName];
}

void BaseDeDatos::AgregarTabla(const NombreTabla& tableName, Tabla table) {
    _tables[tableName] = table;
}

void BaseDeDatos::BorrarTabla(const NombreTabla& tableName) {
    _tables.erase(tableName);
}

void BaseDeDatos::AgregarRegistro(const NombreTabla& tableName, Registro reg) {
    _tables[tableName].insertar(reg);
}

void BaseDeDatos::BorrarRegistro(const NombreTabla& tableName, Registro reg) {
    NombreCampo keyField = _tables[tableName].clave();
    _tables[tableName].borrar(reg[keyField]);
}
