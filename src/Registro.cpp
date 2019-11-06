#include "Registro.h"

Registro::Registro() {
    COMPLETAR(Registro);
}

set<NombreCampo> Registro::campos() const {
    COMPLETAR(campos);
}

Valor& Registro::operator[](const NombreCampo& campo) {
    COMPLETAR(operator[]);
}

