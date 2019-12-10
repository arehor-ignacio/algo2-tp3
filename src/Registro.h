#ifndef __REGISTRO_H__
#define __REGISTRO_H__

#include <set>

#include "Tipos.h"
#include "modulos_basicos/string_map.h"

class Registro {
public:
    const std::set<NombreCampo>& campos() const;
    const Valor& operator[](const NombreCampo&) const;
    bool operator==(const Registro&) const;
    void definir(const NombreCampo&, const Valor&);
    bool def(const NombreCampo&) const;
private:
    string_map<Valor> _registro;
};

#endif /*__REGISTRO_H__*/