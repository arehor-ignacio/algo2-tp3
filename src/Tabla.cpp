#include "Tabla.h"

Tabla::Tabla() {}
Tabla::Tabla(const NombreCampo& k, const std::set<NombreCampo>& campos) : _clave(k) {
    for (const NombreCampo& c : campos) {
        this->_valoresPorCampo[c] = string_map<ConjItRegistros>();
    }
}
/* Complejidad: O(Len(c) + Len(v) + n)*/
void Tabla::insertar(const Registro& r) {
    Valor vClave = r[this->clave()];
    if (this->claveDefinida(vClave)) {
        borrar(vClave);
    }

    ItRegistro it = this->_registros.fast_insert(r);
    for (NombreCampo c : r.campos()) {
        this->_valoresPorCampo.at(c)[r[c]].fast_insert(it);
    }
}
/* Pre: claveDefinida(v) */
/* Complejidad: O(Len(c) + Len(v) + n)*/
void Tabla::borrar(const Valor& v) {
    ItRegistro it = obtenerItRegistro(v);
    for (const NombreCampo& c : this->campos()) {
        this->_valoresPorCampo.at(c).at((*it).operator[](c)).erase(it);
    }
    this->_registros.erase(it);
}
/* Complejidad: O(Len(c) + Len(v) + n *(Len(c) + Copy(v))) */
linear_set<Registro> Tabla::registrosValorEnCampo(const NombreCampo& c, const Valor& v) const {
    const ConjItRegistros& itRegs = this->_valoresPorCampo.at(c).at(v);
    linear_set<Registro> res = linear_set<Registro>();

    for (const ItRegistro& it : itRegs) {
        res.fast_insert(*it);
    }
    return res;
}
/* Complejidad: O(Len(c) + Len(v) + n *(Len(c) + Copy(v))) */
bool Tabla::claveDefinida(const Valor& v) const {
//    return registrosValorEnCampo(this->clave(), v).size() == 1;
    return this->_valoresPorCampo.at(this->clave()).count(v) == 1;
}
/* Pre: claveDefinida(v) */
/* Complejidad: O(Len(c) + Len(v)) */
const ItRegistro& Tabla::obtenerItRegistro(const Valor& v) const {
    const ConjItRegistros& conjItRegistros = this->_valoresPorCampo.at(this->clave()).at(v);
    linear_set<ItRegistro>::const_iterator itConjunto = conjItRegistros.begin();
    return *itConjunto;
}
/* Complejidad: O(1) */
const std::set<NombreCampo>& Tabla::campos() const {
    return this->_valoresPorCampo.keys();
}
/* Complejidad: O(1) */
const linear_set<Registro>& Tabla::registros() const {
    return this->_registros;
}
/* Complejidad: O(1) */
const NombreCampo &Tabla::clave() const {
    return this->_clave;
}

bool Tabla::operator==(const Tabla& t) const {
    return this->clave() == t.clave() && this->registros() == t.registros();
}
