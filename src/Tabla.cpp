#include "Tabla.h"

void Tabla::insertar(Registro reg) {
    auto it = _getNewRegisterIt(reg);
    for (const NombreCampo &field : reg.campos()) {
        Valor value = reg[field];
        _valuesByField[field][value].insert(it);
    }
}

void Tabla::borrar(Valor keyValue) {
    RegisterValue reg = _valuesByField[_key][keyValue];
    if (!reg.empty()) {
        //Obtengo el it del registro asociado al valor del campo clave
        //Como todos los regs tienen valores diferentes en el campo clave, hay un único valor en el set de regs
        auto itRegisterToErase = *(reg.begin());
        _eraseRegisterValues(itRegisterToErase);

        //También lo eliminamos del set de registros
        _registers.erase(itRegisterToErase);
    }
}

const list<NombreCampo> &Tabla::campos() {
    return _valuesByField.keys();
}

const NombreCampo &Tabla::clave() {
    return _key;
}

const set<Registro> &Tabla::registros() {
    return _registers;
}

Tabla::RegisterIt Tabla::_getNewRegisterIt(Registro newReg) {
    //Checkeo si ya estaba definida la key, si lo esta borramos lo que tenia definido.
    //Pues queremos sobreescrbir sus valores
    for (Registro reg : registros()) {
        if (reg[_key] == newReg[_key]) {
            borrar(reg[_key]);
        }
    }

    return _registers.insert(newReg).first;
}

void Tabla::_eraseRegisterValues(RegisterIt itRegisterToErase) {
    //Buscamos en todos los posibles valores si está el iterador, si está lo borramos
    for (const NombreCampo &field : (*this).campos()) {
        for(const Valor &value : _valuesByField[field].keys()) {
            RegisterValue values = _valuesByField[field][value];
            if (values.count(itRegisterToErase) != 0) {
                //Se encuentra el registro
                values.erase(itRegisterToErase);
            }
        }
    }
}
