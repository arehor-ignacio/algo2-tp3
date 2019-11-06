
#include <cassert>
#include <cstdlib>
#include <sstream>

#include "Driver.h"

void Driver::crearTabla(NombreTabla tabla, vector<NombreCampo> campos,
                        NombreCampo clave) {
    COMPLETAR(crearTabla);
}

void Driver::insertarRegistro(NombreTabla t, Registro r) {
    COMPLETAR(insertarRegistro);
}

Respuesta Driver::consultar(const Consulta& q) {
    COMPLETAR(consultar);
}

void Driver::leerDataset(string dataset) {
    ifstream is("datos/" + dataset + ".txt");
    
    vector<string> campos;
    _leerLinea(is, campos);
    crearTabla(dataset, campos, campos[0]);

    int linea = 1;
    while (!is.eof()) {
        vector<string> valores;
        if (!_leerLinea(is, valores)) {
            break;
        }
        if (campos.size() != valores.size()) {
            cerr << "Error leyendo dataset: " << dataset << endl;
            cerr << "El registro en la linea " << linea
                 << " tiene " << valores.size() << " campos"
                 << " pero la tabla declara " << campos.size() << " campos."
                 << endl;
            exit(1);
        }
        Registro r;
        for (size_t i = 0; i < campos.size(); i++) {
            r[campos[i]] = valores[i];
        }
        insertarRegistro(dataset, r);
    }
}

bool Driver::_leerLinea(ifstream& is, vector<string>& valores) const {
    string linea;
    if (!getline(is, linea)) {
        return false;
    }

    istringstream isl(linea);
    string valor;
    while (getline(isl, valor, ',')) {
        valores.push_back(valor);
    }
    return true;
}

