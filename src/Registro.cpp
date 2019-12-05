#include "Registro.h"

linear_set<NombreCampo> Registro::campos() const {
    return _register.keys();
}

Valor& Registro::operator[](const NombreCampo& campo){
    return _register.at(campo);
}

void Registro::definir(const NombreCampo& field, const Valor& value) {
    _register[field] = value;
}

Registro &Registro::operator=(const Registro& aCopiar) {
    this->_register = aCopiar._register;
    return *this;
}

const Valor& Registro::operator[](const NombreCampo& field) const {
    return _register.at(field);
}

bool Registro::def(const NombreCampo& field) const {
    return _register.count(field) == 1;
}
