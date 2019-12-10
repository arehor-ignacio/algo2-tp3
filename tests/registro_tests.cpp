#include "gtest-1.8.1/gtest.h"
#include "../src/Registro.h"

TEST(registro_test, vacio) {
    Registro reg;

    EXPECT_TRUE(reg.campos().empty());
}

TEST(registro_test, op_corchetes) {
    Registro reg;

    reg.definir("Nombre", "Gabriel");
    EXPECT_EQ(reg["Nombre"], "Gabriel");
}

TEST(registro_test, campos) {
    Registro reg;

    reg.definir("Nombre", "Gabriel");
    reg.definir("Nombre", "Nacho1");
    reg.definir("Apellido", "Alonso");

    EXPECT_EQ(reg.campos().size(), 2);
    EXPECT_TRUE(reg.campos().count("Nombre"));
    EXPECT_TRUE(reg.campos().count("Apellido"));
    EXPECT_FALSE(reg.campos().count("DNI"));
}

TEST(registro_test, definir) {
    Registro reg;

    reg.definir("Nombre", "Gabriel");

    EXPECT_EQ(reg["Nombre"], "Gabriel");

    reg.definir("Nombre", "Nacho1");

    EXPECT_EQ(reg["Nombre"], "Nacho1");

    EXPECT_EQ(reg.campos().size(), 1);
}

TEST(registro_test, op_equals) {
    Registro reg1, reg2, reg3;

    reg1.definir("Nombre", "Gabriel");
    reg1.definir("Apellido", "Hayon");
    reg1.definir("DNI", "40729302");

    reg2.definir("Nombre", "Gabriel");
    reg2.definir("Apellido", "Hayon");
    reg2.definir("DNI", "40729302");

    reg3.definir("Nombre", "Nacho1");
    reg3.definir("Apellido", "Alonso");
    reg3.definir("DNI", "Nose");

    EXPECT_TRUE(reg1 == reg2);
    EXPECT_FALSE(reg1 == reg3);
}