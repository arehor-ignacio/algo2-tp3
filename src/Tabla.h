#ifndef TP3_TABLA_H
#define TP3_TABLA_H

#include <set>
#include <list>
#include "Registro.h"
#include "Tipos.h"

class Tabla {
public:
    Tabla(set<NombreCampo> fields, NombreCampo key) : _valuesByField(), _key(key), _registers() {
        _key = key;
        for (NombreCampo field : fields) {
            _valuesByField[field] = string_map<RegisterValue>();
        }
    };

    void insertar(Registro reg);

    void borrar(Valor keyValue);

    const list<NombreCampo>& campos();

    const NombreCampo& clave();

    const set<Registro>& registros();

private:
    typedef set<Registro>::iterator RegisterIt;
    typedef set<RegisterIt> RegisterValue;

    RegisterIt _getNewRegisterIt(Registro newReg);
    void _eraseRegisterValues(RegisterIt it);

    string_map<string_map<RegisterValue>> _valuesByField;
    NombreCampo _key;
    set<Registro> _registers;
};

#endif //TP3_TABLA_H
