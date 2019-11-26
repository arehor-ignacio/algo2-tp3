#include "Registro.h"

linear_set<NombreCampo> Registro::campos() const {
    return _register.keys();
}

Valor Registro::operator[](const NombreCampo& campo) {
    return _register[campo];
}

void Registro::definir(NombreCampo field, Valor value) {
    _register[field] = value;
}
