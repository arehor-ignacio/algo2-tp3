#include "gtest-1.8.1/gtest.h"
#include "../src/BaseDeDatos.h"

TEST(BDD_test, vacio) {
    string_map<int> vacio;

    EXPECT_TRUE(vacio.empty());
    EXPECT_EQ(vacio.count("hola"), 0);
}