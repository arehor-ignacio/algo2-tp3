#ifndef __TABLA_H__
#define __TABLA_H__

#include "Tipos.h"
#include "modulos_basicos/string_map.h"
#include "Registro.h"



typedef linear_set<Registro>::iterator ItRegistro;
typedef linear_set<ItRegistro> ConjItRegistros;

class Tabla {
public:
    Tabla();
    Tabla(const NombreCampo&, const std::set<NombreCampo>&);
    void insertar(const Registro&);
    void borrar(const Valor&);
    const std::set<NombreCampo>& campos() const;
    const linear_set<Registro>& registros() const;
    linear_set<Registro> registrosValorEnCampo(const NombreCampo&, const Valor&) const;
    const NombreCampo& clave() const;
    bool operator==(const Tabla&) const;
private:
    string_map<string_map<ConjItRegistros>> _valoresPorCampo;
    linear_set<Registro> _registros;
    NombreCampo _clave;

    bool claveDefinida (const Valor&) const;
    const ItRegistro& obtenerItRegistro(const Valor&) const;
};

#endif //__TABLA_H__
