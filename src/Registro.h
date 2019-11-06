#ifndef __REGISTRO_H__
#define __REGISTRO_H__

#include <set>

#include "Tipos.h"

using namespace std;

class Registro {
public:
    Registro();
    set<NombreCampo> campos() const;
    Valor& operator[](const NombreCampo& campo);
private:
    // COMPLETAR con la representación privada.
};

#endif /*__REGISTRO_H__*/
