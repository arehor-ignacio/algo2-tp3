#ifndef __TABLA_H__
#define __TABLA_H__

#include "Tipos.h"
#include "modulos_basicos/string_map.h"
#include "Registro.h"

typedef std::list<Registro>::iterator itListaRegistros;

class Tabla {
public:
    Tabla() = default;
    Tabla(const NombreCampo&, const std::set<NombreCampo>&);
    Tabla& operator=(const Tabla&);

    void insertar(Registro);
    void borrar(const Valor&);
    const std::set<NombreCampo>& campos() const;
    const std::list<Registro>& registros() const;
    std::list<Registro> regsValorEnCampo(const NombreCampo&, const Valor&) const;
    const NombreCampo& clave() const;
    bool operator==(const Tabla&) const;
private:
    //set o list?
    string_map<string_map<std::list<itListaRegistros>>> _valoresEnCampo;
    std::list<Registro> _registros;
    NombreCampo _clave;

    const itListaRegistros& ObtenerPorClave(const Valor&) const;
};

#endif //__TABLA_H__