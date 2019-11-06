#include <cassert>
#include <cstdlib>
#include <sstream>

#include "Consulta.h"

Consulta::Consulta(const string& s) :
   _subconsulta1(nullptr),
   _subconsulta2(nullptr)
{
    stringstream is(s);
    typename Consulta::Parser p(is);
    p.leer_consulta(*this);
}

Consulta::Consulta() :
   _subconsulta1(nullptr),
   _subconsulta2(nullptr)
{
}

TipoConsulta Consulta::tipo_consulta() const {
    return _tipo_consulta;
}

NombreTabla Consulta::nombre_tabla() const {
    assert(_tipo_consulta == FROM);
    return _nombre_tabla;
}

NombreCampo Consulta::campo1() const {
    assert(_tipo_consulta == SELECT
        || _tipo_consulta == MATCH
        || _tipo_consulta == RENAME
        );
    return _campo1;
}

NombreCampo Consulta::campo2() const {
    assert(_tipo_consulta == MATCH
        || _tipo_consulta == RENAME
        );
    return _campo2;
}

Valor Consulta::valor() const {
    assert(_tipo_consulta == SELECT);
    return _valor;
}

set<NombreCampo> Consulta::conj_campos() const {
    assert(_tipo_consulta == PROJ);
    return _conj_campos;
}

const Consulta& Consulta::subconsulta1() const {
    assert(_tipo_consulta == SELECT
        || _tipo_consulta == MATCH
        || _tipo_consulta == PROJ
        || _tipo_consulta == RENAME
        || _tipo_consulta == INTER
        || _tipo_consulta == UNION
        || _tipo_consulta == PRODUCT
        );
    return *_subconsulta1;
}

const Consulta& Consulta::subconsulta2() const {
    assert(_tipo_consulta == INTER
        || _tipo_consulta == UNION
        || _tipo_consulta == PRODUCT
        );
    return *_subconsulta2;
}

Consulta::~Consulta() {
    _destruir();
}

bool Consulta::operator==(const Consulta& otra) {
    // Operador de asignación NO IMPLEMENTADO.
    //
    // Si lo necesitan, deben implementarlo.
    assert(false);
}

Consulta::Consulta(const Consulta& otra) {
    // Constructor por copia NO IMPLEMENTADO.
    //
    // Si lo necesitan, deben implementarlo.
    assert(false);
}

void Consulta::_destruir() {
    if (_subconsulta1 != NULL) {
        delete _subconsulta1;
    }
    if (_subconsulta2 != NULL) {
        delete _subconsulta2;
    }
}

Consulta::Parser::Parser(istream& is) : _input(is) {
}

char Consulta::Parser::_peek() {
    return _input.peek();
}

char Consulta::Parser::_get() {
    char c = _input.get();
    _contexto += c;
    return c;
}

void Consulta::Parser::_mostrar_contexto() {
    cerr << "------ Cerca de: ------" << endl;
    const size_t r = 40;
    const size_t i0 = (_contexto.size() > r) ? (_contexto.size() - r) : 0;
    for (size_t i = i0; i < _contexto.size(); i++) {
        cerr << _contexto[i];
    }
    cerr << endl << "------" << endl;
    for (size_t i = 0; i < r; i++) {
        if (!_input.eof()) {
            cerr << _get();
        }
    }
    cerr << "------------" << endl;
}

void Consulta::Parser::leer_consulta(Consulta& q) {
    _leer_consulta(q);
}

void Consulta::Parser::_leer_consulta(Consulta& q) {
    string id = _leer_identificador();
    if (id == "from") {
        return _leer_from(q);
    } else if (id == "select") {
        return _leer_select(q);
    } else if (id == "match") {
        return _leer_match(q);
    } else if (id == "proj") {
        return _leer_proj(q);
    } else if (id == "rename") {
        return _leer_rename(q);
    } else if (id == "inter") {
        return _leer_inter(q);
    } else if (id == "union") {
        return _leer_union(q);
    } else if (id == "product") {
        return _leer_product(q);
    } else {
        _mostrar_contexto();
        cerr << "Tipo de consulta no reconocido: \"" << id << "\"" << endl;
        exit(1);
    }
}

bool Consulta::Parser::_es_blanco(char c) const {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

bool Consulta::Parser::_es_alfanumerico(char c) const {
    return ('0' <= c && c <= '9')
        || ('A' <= c && c <= 'Z')
        || ('a' <= c && c <= 'z')
        || c == '_';
}

void Consulta::Parser::_ignorar_blancos() {
    while (!_input.eof() && _es_blanco(_peek())) {
        _get();
    }
}

string Consulta::Parser::_leer_identificador() {
    _ignorar_blancos();
    string id = "";
    while (!_input.eof() && _es_alfanumerico(_peek())) {
        id += _get();
    }
    if (id == "") {
        _mostrar_contexto();
        cerr << "Se esperaba un identificador." << endl;
        exit(1);
    }
    return id;
}

string Consulta::Parser::_leer_string() {
    _leer_simbolo('\'');
    string str = "";
    while (!_input.eof() && _peek() != '\'') {
        str += _get();
    }
    if (!_input.eof() && _peek() == '\'') {
        _get();
    } else {
        _mostrar_contexto();
        cerr << "Falta cerrar una comilla (')." << endl;
        exit(1);
    }
    return str;
}

void Consulta::Parser::_leer_simbolo(char simbolo) {
    _ignorar_blancos();
    if (!_input.eof() && _peek() == simbolo) {
        _get();
        return;
    }
    _mostrar_contexto();
    cerr << "Se esperaba: \"" << simbolo << "\"." << endl;
    exit(1);
}

set<string> Consulta::Parser::_leer_conj_campos() {
    set<string> campos;

    _leer_simbolo('{');

    // No hay ningún campo.
    _ignorar_blancos();
    if (_peek() == '}') {
        _leer_simbolo('}');
        return campos;
    }

    // Hay al menos un campo.
    campos.insert(_leer_identificador());
    _ignorar_blancos();
    while (_peek() == ',') {
        _leer_simbolo(',');
        campos.insert(_leer_identificador());
        _ignorar_blancos();
    }
    _leer_simbolo('}');
    return campos;
}

void Consulta::Parser::_leer_from(Consulta& q) {
    q._tipo_consulta = FROM;
    _leer_simbolo('(');
    q._nombre_tabla = _leer_identificador();
    _leer_simbolo(')');
}

void Consulta::Parser::_leer_select(Consulta& q) {
    q._tipo_consulta = SELECT;
    _leer_simbolo('(');
    q._subconsulta1 = new Consulta;
    _leer_consulta(*q._subconsulta1);
    _leer_simbolo(',');
    q._campo1 = _leer_identificador();
    _leer_simbolo(',');
    q._valor = _leer_string();
    _leer_simbolo(')');
}

void Consulta::Parser::_leer_match(Consulta& q) {
    q._tipo_consulta = MATCH;
    _leer_simbolo('(');
    q._subconsulta1 = new Consulta;
    _leer_consulta(*q._subconsulta1);
    _leer_simbolo(',');
    q._campo1 = _leer_identificador();
    _leer_simbolo(',');
    q._campo2 = _leer_identificador();
    _leer_simbolo(')');
}

void Consulta::Parser::_leer_proj(Consulta& q) {
    q._tipo_consulta = PROJ;
    _leer_simbolo('(');
    q._subconsulta1 = new Consulta;
    _leer_consulta(*q._subconsulta1);
    _leer_simbolo(',');
    q._conj_campos = _leer_conj_campos();
    _leer_simbolo(')');
}

void Consulta::Parser::_leer_rename(Consulta& q) {
    q._tipo_consulta = RENAME;
    _leer_simbolo('(');
    q._subconsulta1 = new Consulta;
    _leer_consulta(*q._subconsulta1);
    _leer_simbolo(',');
    q._campo1 = _leer_identificador();
    _leer_simbolo(',');
    q._campo2 = _leer_identificador();
    _leer_simbolo(')');
}

void Consulta::Parser::_leer_inter(Consulta& q) {
    q._tipo_consulta = INTER;
    _leer_simbolo('(');
    q._subconsulta1 = new Consulta;
    _leer_consulta(*q._subconsulta1);
    _leer_simbolo(',');
    q._subconsulta2 = new Consulta;
    _leer_consulta(*q._subconsulta2);
    _leer_simbolo(')');
}

void Consulta::Parser::_leer_union(Consulta& q) {
    q._tipo_consulta = UNION;
    _leer_simbolo('(');
    q._subconsulta1 = new Consulta;
    _leer_consulta(*q._subconsulta1);
    _leer_simbolo(',');
    q._subconsulta2 = new Consulta;
    _leer_consulta(*q._subconsulta2);
    _leer_simbolo(')');
}

void Consulta::Parser::_leer_product(Consulta& q) {
    q._tipo_consulta = PRODUCT;
    _leer_simbolo('(');
    q._subconsulta1 = new Consulta;
    _leer_consulta(*q._subconsulta1);
    _leer_simbolo(',');
    q._subconsulta2 = new Consulta;
    _leer_consulta(*q._subconsulta2);
    _leer_simbolo(')');
}

static string mostrar_conj(set<string> v) {
    string res = "";
    bool primero = true;
    for (string elem : v) {
        if (!primero) {
            res += ", ";
        } else {
            primero = false;
        }
        res += elem;
    }
    return res;
}

ostream& operator<<(ostream& os, const Consulta& q) {
    switch (q.tipo_consulta()) {
    case FROM:
        return os << "from(" << q.nombre_tabla() << ")";
    case SELECT:
        return os << "select(" << q.subconsulta1()
                  << ", " << q.campo1()
                  << ", " << "'" << q.valor() << "'"
                  << ")";
    case MATCH:
        return os << "match(" << q.subconsulta1()
                  << ", " << q.campo1()
                  << ", " << q.campo2()
                  << ")";
    case PROJ:
        return os << "proj(" << q.subconsulta1()
                  << ", " << "{" << mostrar_conj(q.conj_campos()) << "}"
                  << ")";
    case RENAME:
        return os << "rename(" << q.subconsulta1()
                  << ", " << q.campo1()
                  << ", " << q.campo2()
                  << ")";
    case INTER:
        return os << "inter(" << q.subconsulta1()
                  << ", " << q.subconsulta2()
                  << ")";
    case UNION:
        return os << "union(" << q.subconsulta1()
                  << ", " << q.subconsulta2()
                  << ")";
    case PRODUCT:
        return os << "product(" << q.subconsulta1()
                  << ", " << q.subconsulta2()
                  << ")";
    }
    return os;
}

