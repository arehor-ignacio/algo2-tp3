#include "Registro.h"

/* Complejidad: O(Copy(c)) */
const set<NombreCampo>& Registro::campos() const {
    return this->_registro.keys();
}

/* Complejidad: O(Len(c)) */
const Valor& Registro::operator[](const NombreCampo& campo) const{
    return this->_registro.at(campo);
}

/* Complejidad: O(Len(c) + Equal(v)) */
bool Registro::operator==(const Registro& r) const {
    return this->_registro == r._registro;
}

/* Complejidad: O(n + Len(c) + Copy(v)) */
void Registro::definir(const NombreCampo& c, const Valor& v) {
    this->_registro[c] = v;
}

/* Complejidad: O(Len(c)) */
bool Registro::def(const NombreCampo& c) const {
    return this->_registro.count(c) == 1;
}