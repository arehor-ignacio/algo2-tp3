#ifndef __DRIVER_H__
#define __DRIVER_H__

#include <fstream>
#include <vector>

#include "Registro.h"
#include "Consulta.h"

typedef vector<Registro> Respuesta;

class Driver {
public:
    // Deben completar los siguientes métodos para invocar a la
    // implementación de la base de datos.
    void crearTabla(NombreTabla tabla,
                    vector<NombreCampo> campos,
                    NombreCampo clave);
    void insertarRegistro(NombreTabla t, Registro r);
    Respuesta consultar(const Consulta& q);

    // El método leerDataset está provisto por la cátedra.
    //
    // Crea una tabla llamada <dataset>
    // y la llena con datos leídos desde un archivo ubicado
    // en el directorio datos/<dataset>.txt
    //
    // El archivo debe estar en formato CSV(*).
    // La primera línea contiene los campos separados por comas.
    // La clave es el primer campo (primera columna de la tabla).
    // Las líneas restantes contienen los registros.
    //
    // (*) El formato CSV soportado es muy básico,
    // por ejemplo no hay ninguna manera de incluir comas en
    // los campos.
    void leerDataset(string dataset);
private:

    // COMPLETAR con la representación privada.
    BaseDeDatos _baseDeDatos;

    bool _leerLinea(ifstream& is, vector<string>& valores) const;
};

#endif /*__DRIVER_H__*/
