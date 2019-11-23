#ifndef TP3_BASEDEDATOS_H
#define TP3_BASEDEDATOS_H

#include "modulos_basicos/string_map.h"

#include "Tabla.h"
#include "Tipos.h"
#include "Registro.h"
#include <list>

class BaseDeDatos {
public:
    BaseDeDatos();

    /**
     * Returns a set with all contained nameTables
     * Time complexity: O(1)
     * */
    const list<NombreTabla> &Tablas() const;

    /**
     * Returns the table associated with the name
     * Time complexity: O(1)
     * */
    Tabla &ObtenerTabla(const NombreTabla& tableName);

    /**
     * Adds a table with tableName as key and table as value
     * Time complexity: O(name.size + copy(table))
     * */
    void AgregarTabla(const NombreTabla& tableName, Tabla table);

    /**
     * Erases the table associated with tableName
     * Time complexity: O(name.size + k * BorarRegistro(tableName, k_i))
     * with k being the amount of registers in the table
     * and k_i every register in the table
     * */
    void BorrarTabla(const NombreTabla& tableName);

    /**
     * Adds the reg in the table associated with tableName
     * Time complexity: O(tableName.size + reg.size * (f.size + v.size))
     * reg.size = O(1), with f being the longest field and v being the longest value
     * */
    void AgregarRegistro(const NombreTabla& tableName, Registro reg);

    /**
     * Erases the reg in the table associated with tableName
     * Time complexity: O(tableName.size + reg.size * f.size)
     * reg.size = O(1), with f being the longest field
     * */
    void BorrarRegistro(const NombreTabla& tableName, Registro reg);

private:
    string_map<Tabla> _tables;

};

#endif //TP3_BASEDEDATOS_H
