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
