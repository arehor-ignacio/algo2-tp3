#ifndef __REGISTRO_H__
#define __REGISTRO_H__

#include <list>

#include "Tipos.h"
#include "modulos_basicos/string_map.h"

using namespace std;

class Registro {
public:
    /**
     * Creates a new Registro
     * Time complexity: O(1)
     * */
    Registro() : _register() {};

    /**
     * Returns the value of the field
     * Time complexity: O(field.size)
     * */
    Valor& operator[](const NombreCampo&);

    const Valor& operator[](const NombreCampo&) const;

    bool operator==(const Registro reg) const{
        return this->_register == reg._register;
    }

    Registro& operator=(const Registro&);

    bool def(const NombreCampo&) const;

    /**
     * Returns a list with all the fields of the register
     * Time complexity: O(_register.keys.size * field)
     * _register.keys.size = O(1)
     * field = O(_register.keys[i].size) with i = longest key position
     * */
    linear_set<NombreCampo> campos() const;

    /**
     * Defines the value in the designed field
     * Time complexity: O(field.size * value.size)
     * */
    void definir(const NombreCampo& field, const Valor& value);
private:
    string_map<Valor> _register;
};

#endif /*__REGISTRO_H__*/
