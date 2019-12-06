#include "gtest-1.8.1/gtest.h"
#include "../src/BaseDeDatos.h"
#include <set>
std::set<NombreCampo> personsTableFields() {
    std::set<NombreCampo> fields = std::set<NombreCampo>();

    fields.insert("Nombre");
    fields.insert("Apellido");
    fields.insert("DNI");

    return fields;
}

std::set<NombreCampo> countryTableFields() {
    std::set<NombreCampo> fields = std::set<NombreCampo>();

    fields.insert("Nombre");
    fields.insert("Cant Personas");
    fields.insert("Continente");

    return fields;
}

Tabla createPersonsTable() {
    return Tabla("DNI", personsTableFields());
}

Tabla createCountriesTable() {
    return Tabla("Nombre", countryTableFields());
}

Registro createRegister(vector<NombreCampo> fields, vector<NombreCampo> values) {
    Registro reg;

    for (int i = 0; i < 3; ++i) {
        reg.definir(fields[i], values[i]);
    }

    return reg;
}

Registro createPersonRegister(vector<NombreCampo> values) {
    vector<NombreCampo> fields = {"Nombre", "Apellido", "DNI"};
    return createRegister(fields, values);
}

Registro createCountryRegister(vector<NombreCampo> values) {
    vector<NombreCampo> fields = {"Nombre", "Cant Personas", "Continente"};
    return createRegister(fields, values);
}

TEST(BDD_test, vacio) {
    BaseDeDatos db;

    EXPECT_EQ(db.tablas().size(), 0);
}

TEST(BDD_test, insertar_y_borrar_tablas) {
    BaseDeDatos db;

    db.agregarTabla("Personas", createPersonsTable());

    EXPECT_EQ(db.tablas().size(), 1);

    db.agregarTabla("Paises", createCountriesTable());

    EXPECT_EQ(db.tablas().size(), 2);

    db.borrarTabla("Personas");

    EXPECT_EQ(db.tablas().size(), 1);

    db.borrarTabla("Paises");

    EXPECT_EQ(db.tablas().size(), 0);
}

TEST(BDD_test, obtener_tabla) {
    BaseDeDatos db;

    db.agregarTabla("Personas", createPersonsTable());
    db.agregarTabla("Paises", createCountriesTable());

    Tabla personsTable = db.obtenerTabla("Personas");

    EXPECT_EQ(createPersonsTable(), personsTable);

    db.borrarTabla("Personas");

    Tabla countriesTable = db.obtenerTabla("Paises");

    EXPECT_EQ(createCountriesTable(), countriesTable);
}

TEST(BDD_test, insertar_y_borrar_registros) {
    BaseDeDatos db;

    Registro regPer1, regPer2, regCou1;

    regPer1 = createPersonRegister({"Gabriel", "Hayon", "40729302"});
    regPer2 = createPersonRegister({"Nacho1", "Alonso", "12345678"});
    regCou1 = createCountryRegister({"Argentina", "45000000", "Sudamerica"});

    db.agregarTabla("Personas", createPersonsTable());
    db.agregarTabla("Paises", createCountriesTable());

    db.agregarRegistro("Personas", regPer1);
    db.agregarRegistro("Personas", regPer2);
    db.agregarRegistro("Paises", regCou1);

    linear_set<Registro> personsRegs, countriesRegs;

    personsRegs.insert(regPer1);
    personsRegs.insert(regPer2);

    countriesRegs.insert(regCou1);

    EXPECT_EQ(db.obtenerTabla("Personas").registros(), personsRegs);
    EXPECT_EQ(db.obtenerTabla("Paises").registros(), countriesRegs);

    db.borrarRegistro("Personas", regPer1["DNI"]);
    personsRegs.erase(regPer1);

    EXPECT_EQ(db.obtenerTabla("Personas").registros(), personsRegs);

    db.borrarRegistro("Paises", regCou1["Nombre"]);
    countriesRegs.erase(regCou1);

    EXPECT_EQ(db.obtenerTabla("Paises").registros(), countriesRegs);
}