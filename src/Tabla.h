#ifndef TP3_TABLA_H
#define TP3_TABLA_H

#include <set>
#include <list>
#include "Registro.h"
#include "Tipos.h"
#include "modulos_basicos/linear_set.h"

class Tabla {
public:
    //No debe ser usado, solo creado para compatibilidad con string_map
    Tabla() : _valuesByField(), _key(), _registers() {};

    Tabla(linear_set<NombreCampo> fields, NombreCampo key) : _valuesByField(), _key(key), _registers() {
        _key = key;
        for (NombreCampo field : fields) {
            _valuesByField[field] = string_map<RegisterValue>();
        }
    };

    bool operator==(const Tabla table) const {
        return this->_valuesByField == table._valuesByField && this->_key == table._key &&
               this->_registers == table._registers;
    }

    void insertar(Registro reg);

    void borrar(Valor keyValue);

    const linear_set<NombreCampo> &campos() const;

    const NombreCampo &clave() const;

    const linear_set<Registro> &registros() const;

    linear_set<Registro> regsByFieldAndValue(const NombreCampo field, const Valor value) const;

private:
    typedef linear_set<Registro>::iterator RegisterIt;
    typedef linear_set<RegisterIt> RegisterValue;

    RegisterIt _getNewRegisterIt(Registro newReg);

    void _eraseRegisterValues(RegisterIt it);

    string_map<string_map<RegisterValue>> _valuesByField;
    NombreCampo _key;
    linear_set<Registro> _registers;
};

#endif //TP3_TABLA_H
