#include "gtest-1.8.1/gtest.h"
#include "../src/Tabla.h"
#include <vector>

std::set<NombreCampo> getFieldsToTest() {
    std::set<NombreCampo> fields;

    fields.insert("Nombre");
    fields.insert("Apellido");
    fields.insert("Domicilio");
    fields.insert("DNI");

    return fields;
}

NombreCampo getKeyToTest() {
    return "DNI";
}

Tabla initializeTable() {
    return Tabla(getKeyToTest(), getFieldsToTest());
}

Registro createRegister(vector<NombreCampo> values) {
    vector<NombreCampo> fields = {"Nombre", "Apellido", "Domicilio", "DNI"};
    Registro reg;

    for (int i = 0; i < 4; ++i) {
        reg.definir(fields[i], values[i]);
    }

    return reg;
}

TEST(tabla_test, vacio) {
    Tabla table = initializeTable();

    EXPECT_EQ(table.registros().size(), (int) 0);
    EXPECT_EQ(table.campos(), getFieldsToTest());
    EXPECT_EQ(table.clave(), getKeyToTest());
}

TEST(tabla_test, insertar_y_borrar) {

    Tabla table = initializeTable();

    Registro reg1 = createRegister({"Gabriel", "Hayon", "Bs as", "40729302"});
    Registro reg2 = createRegister({"Nacho1", "Alonso", "Bs as", "12345678"});
    Registro reg3 = createRegister({"Nacho2", "Crinigan", "Bs as", "87654321"});

    std::list<Registro> regs;

    table.insertar(reg1);
    regs.push_back(reg1);

    EXPECT_EQ(table.registros().size(), 1);
    EXPECT_EQ(table.registros(), regs);

    table.insertar(reg2);
    regs.push_back(reg2);

    EXPECT_EQ(table.registros().size(), 2);
    EXPECT_EQ(table.registros(), regs);

    table.insertar(reg3);
    regs.push_back(reg3);

    EXPECT_EQ(table.registros().size(), 3);
    EXPECT_EQ(table.registros(), regs);

    table.borrar("12345678"); //reg2
    regs.remove(reg2);

    EXPECT_EQ(table.registros().size(), 2);
    EXPECT_EQ(table.registros(), regs);

    table.borrar("40729302"); //reg1
    regs.remove(reg1);

    EXPECT_EQ(table.registros().size(), 1);
    EXPECT_EQ(table.registros(), regs);

    table.borrar("87654321"); //reg3
    regs.remove(reg3);

    EXPECT_EQ(table.registros().size(), 0);
    EXPECT_EQ(table.registros(), regs);

    EXPECT_EQ(table.campos(), getFieldsToTest());
    EXPECT_EQ(table.clave(), getKeyToTest());
}