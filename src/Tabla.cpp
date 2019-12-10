#include "Tabla.h"

Tabla::Tabla(const NombreCampo& k, const std::set<NombreCampo>& cs) : _clave(k) {
    for (NombreCampo c : cs) {
        this->_valoresEnCampo[c] = string_map<std::list<itListaRegistros>>();
    }
}
/* Complejidad: O(Copy(c) + n * (Len(c) + Copy(v))) */
Tabla& Tabla::operator=(const Tabla& t) {
    this->_clave = t.clave();

    for (const NombreCampo& c : t.campos()) {
        this->_valoresEnCampo[c] = string_map<std::list<itListaRegistros>>();
    }

    for (const Registro& r : t.registros()) {
        this->insertar(r);
    }
    return *this;
}

/* Complejidad: O(Len(c) + Copy(v) + Len(v) + n)*/
void Tabla::insertar(Registro r) {
    std::list<Registro>::const_iterator itEnd = this->_registros.end();
    std::list<Registro>::iterator itInsertar = this->_registros.insert(itEnd, r);

    for (NombreCampo c : r.campos()) {
        Valor val = r[c];
        this->_valoresEnCampo.at(c)[val].push_back(itInsertar); // Agregamos al final del "conjunto" el iterador al registro.
    }
}

/* Complejidad: O(Len(c) + Len(v) + n)*/
void Tabla::borrar(const Valor& v) {
    itListaRegistros itRegistro = ObtenerPorClave(v);

    for (NombreCampo c : this->campos()) {
        Valor val = (*itRegistro)[c];
        this->_valoresEnCampo.at(c).at(val).remove(itRegistro);
    }
    this->_registros.erase(itRegistro);
}

/* Complejidad: O(1) */
const std::set<NombreCampo>& Tabla::campos() const {
    return this->_valoresEnCampo.keys();
}

/* Complejidad: O(1) */
const std::list<Registro>& Tabla::registros() const {
    return this->_registros;
}
/* Complejidad: O(Len(c) + Len(v) + n * (Len(c) + Copy(v))) */
std::list<Registro> Tabla::regsValorEnCampo(const NombreCampo& c, const Valor& v) const {
    const std::list<itListaRegistros>& conjIteradores = this->_valoresEnCampo.at(c).at(v);
    std::list<Registro> res;

    for (itListaRegistros it : conjIteradores) {
        res.push_back(*it);
    }
    return res;
}

/* Complejidad: O(1) */
const NombreCampo& Tabla::clave() const {
    return this->_clave;
}

/* Complejidad: O(Equal(c) + n * (Len(c) + Equal(v)))*/
bool Tabla::operator==(const Tabla& t) const {
    return this->clave() == t.clave() && this->registros() == t.registros();
}

/* Complejidad: O(Len(c) + Len(v)) */
const itListaRegistros& Tabla::ObtenerPorClave(const Valor& v) const{
    const std::list<itListaRegistros>& conjItRegistro = this->_valoresEnCampo.at(this->clave()).at(v);
    return conjItRegistro.front(); // Como solo hay un único elemento cuyo campo clave vale v.
}