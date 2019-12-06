#include "Registro.h"

/* Complejidad: O(1) */
Registro::Registro() : _registro(algo2::string_map<Valor>()){
}
/* Complejidad: O(Copy(c)) */
set<NombreCampo> Registro::campos() const {
    return this->_registro.keys();
}
/* Complejidad: O(Len(c)) */
Valor& Registro::operator[](const NombreCampo& campo){
    return this->_registro.at(campo);
}
/* Complejidad: O(Len(c)) */
const Valor& Registro::operator[](const NombreCampo& campo) const {
    return this->_registro.at(campo);
};
/* Complejidad: O(Len(c)) */
bool Registro::operator==(const Registro& r) const {
    return this->_registro == r._registro;
}

/* Complejidad: O(n + Len(c) + Copy(v)) */
void Registro::definir(const NombreCampo& campo, const Valor& v) {
    this->_registro[campo] = v;
}
/* Complejidad: O(Equal(c)) */
bool Registro::def(const NombreCampo& campo) const {
    return this->_registro.count(campo) == 1;
}


