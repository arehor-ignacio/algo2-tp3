#ifndef __REGISTRO_H__
#define __REGISTRO_H__


#include "Tipos.h"
#include <set>
#include "modulos_basicos/string_map.h"

using namespace std;

class Registro {
public:
    Registro();
    Valor& operator[](const NombreCampo&);
    const Valor& operator[](const NombreCampo&) const;
    bool operator==(const Registro&) const;
    void definir(const NombreCampo&, const Valor&);
    set<NombreCampo> campos() const;
    bool def(const NombreCampo&) const;
private:
    algo2::string_map<Valor> _registro;
};

#endif /*__REGISTRO_H__*/
