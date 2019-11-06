#ifndef __CONSULTA_H__
#define __CONSULTA_H__

#include <iostream>
#include <set>

#include "Tipos.h"

using namespace std;

enum TipoConsulta {
    FROM,
    SELECT,
    MATCH,
    PROJ,
    RENAME,
    INTER,
    UNION,
    PRODUCT,
};

class Consulta {
public:
    // El constructor recibe un string que representa
    // la consulta, y la construye apropiadamente.
    //
    // La sintaxis de las consultas es la siguiente:
    //
    //   from(nombre_tabla)
    //   select(consulta, nombre_campo, valor)
    //   match(consulta, nombre_campo1, nombre_campo2)
    //   proj(consulta, {nombre_campo1, ..., nombre_campoN})
    //   rename(consulta, nombre_campo1, nombre_campo2)
    //   inter(consulta1, consulta2)
    //   union(consulta1, consulta2)
    //   product(consulta1, consulta2)
    //
    // Los nombres de las tablas y los campos pueden incluir
    // caracteres alfabéticos (a..z, A..Z), numéricos (0..9)
    // y guión bajo.
    //
    // Los valores son strings encerrados entre comillas simples (')
    // por ejemplo: 'Fulano de Tal' es un valor.
    //
    // Ejemplos de consultas:
    //    Consulta q1("select(from(empleados), nombre, 'fulano')")
    //    Consulta q2("proj(from(empleados), {nombre, apellido})")
    //    Consulta q3("product(from(empleados), from(sueldos))")
    //
    Consulta(const string&);

    // Los observadores son los esperados.
    //
    TipoConsulta tipo_consulta() const;
    NombreTabla nombre_tabla() const;
    NombreCampo campo1() const;
    NombreCampo campo2() const;
    Valor valor() const;
    set<NombreCampo> conj_campos() const;
    const Consulta& subconsulta1() const;
    const Consulta& subconsulta2() const;

    ~Consulta();

private:
    TipoConsulta _tipo_consulta;
    NombreTabla _nombre_tabla;
    NombreCampo _campo1;
    NombreCampo _campo2;
    Valor _valor;
    set<NombreCampo> _conj_campos;
    Consulta* _subconsulta1;
    Consulta* _subconsulta2;

    Consulta();

    // Operador de asignación y constructor por copia no implementados.
    bool operator==(const Consulta&);
    Consulta(const Consulta&);

    void _leer_de(istream&);
    void _destruir();

    class Parser {
    public:
        Parser(istream&);
        void leer_consulta(Consulta&);
    private:
        char _peek();
        char _get();

        bool _es_blanco(char c) const;
        bool _es_alfanumerico(char c) const;

        void _mostrar_contexto();

        void _ignorar_blancos();
        void _leer_simbolo(char simbolo);
        string _leer_identificador();
        string _leer_string();
        set<string> _leer_conj_campos();

        void _leer_consulta(Consulta&);
        void _leer_from(Consulta&);
        void _leer_select(Consulta&);
        void _leer_match(Consulta&);
        void _leer_proj(Consulta&);
        void _leer_rename(Consulta&);
        void _leer_inter(Consulta&);
        void _leer_union(Consulta&);
        void _leer_product(Consulta&);

        istream& _input;
        string _contexto;
    };
};

ostream& operator<<(ostream&, const Consulta&);

#endif /*__CONSULTA_H__*/

