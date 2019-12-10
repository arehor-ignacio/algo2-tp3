#ifndef __BASEDEDATOS_H__
#define __BASEDEDATOS_H__

#include "Tipos.h"
#include "modulos_basicos/string_map.h"
#include "Registro.h"
#include "Tabla.h"

class BaseDeDatos {
public:
    const string_map<Tabla>& tablas() const;
    const Tabla& obtenerTabla(const NombreTabla&) const;
    void agregarTabla(const NombreTabla&, const Tabla&);
    void borrarTabla(const NombreTabla&);
    void agregarRegistro(const NombreTabla&, const Registro&);
    void borrarRegistro(const NombreTabla&, const Valor&);
private:
    string_map<Tabla> _tablas;
};

#endif //__BASEDEDATOS_H__