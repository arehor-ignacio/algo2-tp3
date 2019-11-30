#include "Registro.h"

linear_set<NombreCampo> Registro::campos() const {
    return _register.keys();
}

const Valor& Registro::operator[](const NombreCampo& campo) const {
    return _register.at(campo);
}

void Registro::definir(const NombreCampo& field, const Valor& value) {
    _register[field] = value;
}
