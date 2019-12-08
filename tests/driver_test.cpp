#include "gtest-1.8.1/gtest.h"
#include "../src/Driver.h"
#include "../src/modulos_basicos/linear_set.h"

set<string> proj1(const Respuesta& r, NombreCampo c) {
    set<string> s;
    for (Registro reg : r) {
        s.insert(reg[c]);
    }
    return s;
}

set<string> proj2(const Respuesta& r, NombreCampo c1, NombreCampo c2) {
    set<string> s;
    for (Registro reg : r) {
        s.insert(reg[c1] + "," + reg[c2]);
    }
    return s;
}

set<string> proj4(const Respuesta& r, NombreCampo c1, NombreCampo c2,
                                      NombreCampo c3, NombreCampo c4) {
    set<string> s;
    for (Registro reg : r) {
        s.insert(reg[c1] + "," + reg[c2] + "," + reg[c3] + "," + reg[c4]);
    }
    return s;
}

TEST(test_bd, test_from_basico) {
    Driver d;
    d.leerDataset("personas");
    Respuesta r = d.consultar(Consulta("from(personas)"));
    ASSERT_EQ(r.size(), (size_t)3);
}

TEST(test_bd, test_select_basico) {
    Driver d;
    d.leerDataset("personas");
    Respuesta r = d.consultar(Consulta(
      "select(from(personas), nombre, 'fulano')"
    ));
    ASSERT_EQ(r.size(), (size_t)1);
    ASSERT_EQ(r[0]["cuit"], "101");
    ASSERT_EQ(r[0]["nombre"], "fulano");
    ASSERT_EQ(r[0]["apellido"], "de tal");
}

TEST(test_bd, test_select_avanzado) {
    Driver d;
    d.leerDataset("trenes");

    //Consulta q = Consulta("select(from(trenes), linea, 'roca')");
    //Respuesta r = d.consultar(q);

    Respuesta r1 = d.consultar(Consulta(
        "select(from(trenes), linea, 'roca')"
        ));
    d.leerDataset("select_linea_roca");
    Respuesta r2 = d.consultar(Consulta(
        "from(select_linea_roca)"
        ));
    ASSERT_EQ(r1.size(), (size_t)371);
    ASSERT_EQ(proj4(r1, "mes", "linea", "estacion", "cantidad"), 
              proj4(r2, "mes", "linea", "estacion", "cantidad"));

    Respuesta r3 = d.consultar(Consulta(
        "select(select(from(trenes), linea, 'roca'), estacion, 'Temperley')"
        ));

    d.leerDataset("select_linea_roca_temperley");
    Respuesta r4 = d.consultar(Consulta(
        "from(select_linea_roca_temperley)"
        ));
    ASSERT_EQ(r3.size(), (size_t)5);
    ASSERT_EQ(proj4(r3, "mes", "linea", "estacion", "cantidad"), 
              proj4(r4, "mes", "linea", "estacion", "cantidad"));

}

TEST(test_bd, test_select_idempotente) {
    Driver d;
    d.leerDataset("trenes");
    Respuesta r1 = d.consultar(Consulta(
        "select(from(trenes), linea, 'Sarmiento')"
    ));
    Respuesta r2 = d.consultar(Consulta(
        "select("
          "select(from(trenes), linea, 'Sarmiento'),"
          "linea, 'Sarmiento'"
        ")"
    ));
    ASSERT_EQ(proj1(r1, "id"), proj1(r2, "id"));
}

TEST(test_bd, test_select_con_clave) {
    Driver d;
    d.leerDataset("trenes");
    Respuesta r = d.consultar(Consulta(
        "select(from(trenes), id, '1030')"
    ));
    ASSERT_EQ(proj1(r, "cantidad"), set<string>({"41845"}));
}

TEST(test_bd, test_select_sin_clave) {
    Driver d;
    d.leerDataset("trenes");
    Respuesta r = d.consultar(Consulta(
        "select(from(trenes), linea, 'belgranosur')"
    ));
    ASSERT_EQ(r.size(), (size_t)122);
}

TEST(test_bd, test_select_con_clave_de_select_sin_clave) {
    Driver d;
    d.leerDataset("trenes");
    Respuesta r = d.consultar(Consulta(
        "select("
          "select(from(trenes), linea, 'roca'),"
          "id, '140'"
        ")"
    ));
    ASSERT_EQ(proj1(r, "cantidad"), set<string>({"32465"}));
}

TEST(test_bd, test_select_vacio) {
    Driver d;
    d.leerDataset("trenes");
    Respuesta r = d.consultar(Consulta(
        "select("
          "select(from(trenes), linea, 'Sarmiento'),"
          "linea, 'Roca'"
        ")"
    ));
    ASSERT_EQ(r.size(), (size_t)0);
}

TEST(test_bd, test_match_basico) {
    Driver d;
    d.leerDataset("personas");
    Respuesta r = d.consultar(Consulta(
      "match(from(personas), nombre, apellido)"
    ));
    ASSERT_EQ(r.size(), (size_t)1);
    ASSERT_EQ(r[0]["cuit"], "102");
    ASSERT_EQ(r[0]["nombre"], "mengano");
    ASSERT_EQ(r[0]["apellido"], "mengano");
}

TEST(test_bd, test_match_avanzado) {
    Driver d;
    d.leerDataset("interurbano");
    Respuesta r = d.consultar(Consulta(
        "match(from(interurbano), provincia_origen, provincia_destino)"
        ));
    ASSERT_EQ(r.size(), (size_t)46);
}

TEST(test_bd, test_match_idempotente) {
    Driver d;
    d.leerDataset("interurbano");
    Respuesta r1 = d.consultar(Consulta(
        "match(from(interurbano), provincia_origen, provincia_destino)"
        ));
    Respuesta r2 = d.consultar(Consulta(
        "match("
          "match(from(interurbano), provincia_origen, provincia_destino),"
          "provincia_origen,"
          "provincia_destino"
        ")"
        ));
    ASSERT_EQ(proj1(r1, "id"), proj1(r2, "id"));
}

TEST(test_bd, test_match_vacio) {
    Driver d;
    d.leerDataset("trenes");
    Respuesta r = d.consultar(Consulta(
        "match(from(trenes), linea, estacion)"
        ));
    ASSERT_EQ(r.size(), (size_t)0);
}

TEST(test_bd, test_proj_basico) {
    Driver d;
    d.leerDataset("personas");
    Respuesta r = d.consultar(Consulta(
      "proj(from(personas), {cuit})"
    ));
    ASSERT_EQ(r.size(), (size_t)3);

    ASSERT_EQ(proj1(r, "cuit"), set<string>({"101", "102", "103"}));
}

TEST(test_bd, test_proj_avanzado) {
    Driver d;
    d.leerDataset("trenes");
    Respuesta r1 = d.consultar(Consulta(
        "proj(select(from(trenes), linea, 'roca'), {id, estacion, cantidad})"
    ));

    d.leerDataset("proj_select_estacion_cantidad");
    Respuesta r2 = d.consultar(Consulta(
        "from(proj_select_estacion_cantidad)"));

    ASSERT_EQ(r1.size(), (size_t)371);
    ASSERT_EQ(proj2(r1, "estacion", "cantidad"), 
              proj2(r2, "estacion", "cantidad"));
}

TEST(test_bd, test_rename_basico) {
    Driver d;
    d.leerDataset("personas");
    Respuesta r = d.consultar(Consulta(
      "rename("
        "select(rename(from(personas), cuit, id), nombre, 'zutano'),"
        "nombre,"
        "cuit"
      ")"
    ));
    ASSERT_EQ(r.size(), (size_t)1);
    ASSERT_EQ(r[0]["id"], "103");
    ASSERT_EQ(r[0]["cuit"], "zutano");
}

TEST(test_bd, test_rename_avanzado) {
    Driver d;
    d.leerDataset("trenes");
    Respuesta r = d.consultar(Consulta(
      "rename("
        "select("
          "select(from(trenes), estacion, 'Temperley'),"
          "cantidad, '237125'"
        "),"
        "estacion, estacion_tren"
      ")"
    ));
    ASSERT_EQ(r.size(), (size_t)1);
    ASSERT_EQ(r[0]["mes"], "12/2018");
    ASSERT_EQ(r[0]["linea"], "roca");
    ASSERT_EQ(r[0]["estacion_tren"], "Temperley");
    ASSERT_EQ(r[0]["cantidad"],"237125");
}

TEST(test_bd, test_inter_basico) {
    Driver d;
    d.leerDataset("conjunto1");
    d.leerDataset("conjunto2");
    Respuesta r = d.consultar(Consulta(
      "inter(from(conjunto1), from(conjunto2))"
    ));
    ASSERT_EQ(proj1(r, "elemento"), set<string>({"B", "D"}));
}

TEST(test_bd, test_inter_avanzado) {
    Driver d;
    d.leerDataset("trenes");
    Respuesta r1 = d.consultar(Consulta(
        "inter("
          "select(from(trenes), linea, 'mitre'),"
          "select(from(trenes), estacion, 'Retiro')"
        ")"
    ));
    d.leerDataset("inter_mitre_retiro");
    Respuesta r2 = d.consultar(Consulta(
        "from(inter_mitre_retiro)"
    ));

    ASSERT_EQ(proj4(r1, "mes", "linea", "estacion", "cantidad"), 
              proj4(r2, "mes", "linea", "estacion", "cantidad"));    
}

TEST(test_bd, test_union_basico) {
    Driver d;
    d.leerDataset("conjunto1");
    d.leerDataset("conjunto2");
    Respuesta r = d.consultar(Consulta(
      "union(from(conjunto1), from(conjunto2))"
    ));
    ASSERT_EQ(proj1(r, "elemento"),
              set<string>({"A", "B", "C", "D", "E", "F"}));
}

TEST(test_bd, test_union_avanzado) {
    Driver d;
    d.leerDataset("trenes");
    Respuesta r1 = d.consultar(Consulta(
        "union("
          "select(from(trenes), estacion, 'San Isidro'),"
          "select(from(trenes), estacion, 'Temperley')"
        ")"
    ));
    d.leerDataset("union_temperley_sanIsidro");
    Respuesta r2 = d.consultar(Consulta(
        "from(union_temperley_sanIsidro)"));
    ASSERT_EQ(proj4(r1, "mes", "linea", "estacion", "cantidad"), 
              proj4(r2, "mes", "linea", "estacion", "cantidad")); 
}

TEST(test_bd, test_product_basico) {
    Driver d;
    d.leerDataset("conjunto1");
    d.leerDataset("conjunto2");
    Respuesta r = d.consultar(Consulta(
      "product("
      "  rename(from(conjunto1), elemento, primero),"
      "  rename(from(conjunto2), elemento, segundo)"
      ")"
    ));
    ASSERT_EQ(proj2(r, "primero", "segundo"),
              set<string>({
                "A,B", "A,D", "A,E", "A,F",
                "B,B", "B,D", "B,E", "B,F",
                "C,B", "C,D", "C,E", "C,F",
                "D,B", "D,D", "D,E", "D,F",
              }));
}

TEST(test_bd, test_join_con_claves) {
    Driver d;
    d.leerDataset("viajes_corto");
    d.leerDataset("viajes_corto_cantidad");
    d.leerDataset("viajes_corto_estacion");
    Respuesta r1 = d.consultar(Consulta(
      "from(viajes_corto)"
    ));
    Respuesta r2 = d.consultar(Consulta(
      "match("
        "product("
          "from(viajes_corto_cantidad),"
          "from(viajes_corto_estacion)"
        "),"
        "id_viaje_cantidad, id_viaje_estacion"
      ")"
    ));
    ASSERT_EQ(proj4(r1, "mes", "id_linea", "id_estacion", "cantidad"), 
              proj4(r2, "mes", "id_linea", "id_estacion", "cantidad"));
}

TEST(test_bd, test_select_con_clave_product) {
    Driver d;
    d.leerDataset("lineas");
    d.leerDataset("estaciones");
    Respuesta r = d.consultar(Consulta(
      "select("
        "product("
          "from(lineas),"
          "from(estaciones)"
        "),"
        "id_linea, '1'"
      ")"
    ));
    ASSERT_EQ(r.size(), (size_t)212);
}

TEST(test_bd, test_join_sin_claves) {
    Driver d;
    d.leerDataset("viajes_corto");
    d.leerDataset("lineas");
    Respuesta r = d.consultar(Consulta(
      "select("
        "match("
          "product("
            "rename(from(viajes_corto), id_linea, id_linea_1),"
            "rename(from(lineas), id_linea, id_linea_2)"
          "),"
          "id_linea_1, id_linea_2"
        "),"
        "id_viaje, '97'"
      ")"
    ));
    ASSERT_EQ(proj1(r, "nombre_linea"), set<string>({"Roca"}));
}

TEST(test_bd, test_inter_tablas_distintas) {
    Driver d;
    d.leerDataset("select_linea_roca");
    d.leerDataset("select_estacion_constitucion");
    Respuesta r1 = d.consultar(Consulta(
      "select("
        "from(select_linea_roca),"
        "estacion, 'Constitucion'"
      ")"
    ));
    Respuesta r2 = d.consultar(Consulta(
      "inter("
        "from(select_linea_roca),"
        "from(select_estacion_constitucion)"
      ")"
    ));
    Respuesta r3 = d.consultar(Consulta(
      "inter("
        "from(select_estacion_constitucion),"
        "from(select_linea_roca)"
      ")"
    ));
    ASSERT_EQ(proj4(r1, "mes", "linea", "estacion", "cantidad"),
              proj4(r2, "mes", "linea", "estacion", "cantidad"));
    ASSERT_EQ(proj4(r1, "mes", "linea", "estacion", "cantidad"),
              proj4(r3, "mes", "linea", "estacion", "cantidad"));
}

TEST(linear_set, erase) {
  linear_set<int> ls = {1,2,3,4,5,6,7};
  auto it = ls.begin();
  it = ls.erase(it);
  ASSERT_EQ(ls, linear_set<int>({2,3,4,5,6,7}));
}
