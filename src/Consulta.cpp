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

std::list<Registro> Consulta::procesarConsulta(const BaseDeDatos& d) const {
    switch (this->tipo_consulta()) {
        case FROM:
            return procesarFrom(d);
        case SELECT:
            return procesarSelect(d);
        case MATCH:
            return procesarMatch(d);
        case PROJ:
            return procesarProj(d);
        case RENAME:
            return procesarRename(d);
        case INTER:
            return procesarInter(d);
        case UNION:
            return procesarUnion(d);
        case PRODUCT:
            return procesarProduct(d);
    }
    return std::list<Registro>();
}

/** ================================================================================================================ **/
/**                                                   ..: FROM :..                                                   **/
/** ================================================================================================================ **/

/* Descripción: Retorna una copia de los registros de la tabla n en
 * la base de datos b.
 * Complejidad: O(Len(n) + k *(Copy(c) + Copy(v)))
 * Justificación: 'k' es la cantidad de registros, 'c' es el nombre de
 * campo más largo y 'v' es el valor más largo.
 */
const std::list<Registro>& Consulta::procesarFrom(const BaseDeDatos& d) const {
    const NombreTabla& n = this->nombre_tabla();
    return d.obtenerTabla(n).registros();
}

/** ================================================================================================================ **/
/**                                                  ..: SELECT :..                                                  **/
/** ================================================================================================================ **/

/* Descripción: SELECT
 * Complejidad: O(SELECT)
 * Justificación:.
 */
std::list<Registro> Consulta::procesarSelect(const BaseDeDatos& d) const {
    if (this->subconsulta1().tipo_consulta() == PRODUCT &&
        this->subconsulta1().subconsulta1().tipo_consulta() == FROM &&
        this->subconsulta1().subconsulta2().tipo_consulta() == FROM &&
        this->subconsulta1().subconsulta1().nombre_tabla() !=
        this->subconsulta1().subconsulta2().nombre_tabla() &&
        d.obtenerTabla(this->subconsulta1().subconsulta1().nombre_tabla()).clave() == this->campo1()) {
        return procesarSelectProduct(d);
    }
    if (this->subconsulta1().tipo_consulta() == SELECT &&
        this->subconsulta1().subconsulta1().tipo_consulta() == FROM &&
        d.obtenerTabla(this->subconsulta1().subconsulta1().nombre_tabla()).clave() == this->campo1() &&
        d.obtenerTabla(this->subconsulta1().subconsulta1().nombre_tabla()).clave() != this->subconsulta1().campo1()) {
        return procesarSelectSelect(d);
    }
    if (this->subconsulta1().tipo_consulta() == FROM) {
        if (d.obtenerTabla(this->subconsulta1().nombre_tabla()).clave() == this->campo1())
            return procesarSelectConClave(d);
        else
            return procesarSelectSinClave(d);
    }
    return procesarSelectBasico(d);
}

/* Descripción: Retorna una copia de los registros de la tabla 'n' cuyo
 * campo 'k' vale 'v'.
 * Complejidad: O(Len(n) + Len(k) + Copy(c) + Copy(v))
 * Justificación: Como 'k' es el campo clave, como mucho hay que copiar
 * un solo registro. El costo se justifica si 'c' es el nombre de campo
 * más largo y 'v' es el valor más largo. Tener en cuenta que
 * Len(k) < Copy(c) ya que copiar un string tiene el mismo costo que
 * recorrerlo.
 */
std::list<Registro> Consulta::procesarSelectConClave(const BaseDeDatos& d) const {
    const NombreTabla& nombreTabla = this->subconsulta1().nombre_tabla();
    return d.obtenerTabla(nombreTabla).regsValorEnCampo(this->campo1(), this->valor());
}

/* Descripción: Retorna una copia de los registros de la tabla 'n' cuyo
 * campo 'c' vale 'v'.
 * Complejidad: O(Len(n) + Len(c) + k * (Copy(f) + Copy(v)))
 * Justificación: A diferencia del algoritmo anterior, es posible que
 * tengamos que devolver más de un registro. Luego si 'k' es la cantidad
 * de registros cuyo campo 'f' vale 'v', el costo del acceso más el del
 * copiado es el que calculamos.
 */
std::list<Registro> Consulta::procesarSelectSinClave(const BaseDeDatos& d) const {
    const NombreTabla& nombreTabla = this->subconsulta1().nombre_tabla();
    return d.obtenerTabla(nombreTabla).regsValorEnCampo(this->campo1(), this->valor());
}

/* Descripción:
 * Complejidad:
 * Justificación:
 */
std::list<Registro> Consulta::procesarSelectProduct(const BaseDeDatos& d) const {
    const NombreTabla& t1 = this->subconsulta1().subconsulta1().nombre_tabla();
    const std::list<Registro>& conjCampoClave =
            d.obtenerTabla(t1).regsValorEnCampo(this->campo1(), this->valor()); /* O(Len(n) + Len(c) + Len(v)) */
    
    const Consulta& query = this->subconsulta1().subconsulta2();
    const std::list<Registro>& resSubProduct= query.procesarConsulta(d);               /* O(Q) */

    std::list<Registro> res;

    for (Registro r : resSubProduct) {                              /* O(n2) */
        res.push_back(pCartesiano(conjCampoClave.front(), r));      /* O(Len(c) + Copy(v)) */
    }

    return res;
}

/* Descripción:
 * Complejidad:
 * Justificación:
 */
std::list<Registro> Consulta::procesarSelectSelect(const BaseDeDatos& d) const {
    const Tabla& t = d.obtenerTabla(this->subconsulta1().subconsulta1().nombre_tabla());
    const std::list<Registro>& resSelect = t.regsValorEnCampo(this->campo1(), this->valor());

    if (resSelect.size() == 1 &&
        resSelect.front()[this->subconsulta1().campo1()] == this->subconsulta1().valor()) {
        return resSelect;
    }
    else return std::list<Registro>(); // Conjunto vacío
}

/* Complejidad: O(n * (Len(c) + Equal(v) + Copy(v))) */
std::list<Registro> Consulta::procesarSelectBasico(const BaseDeDatos& d) const {
    const std::list<Registro>& rSub = this->subconsulta1().procesarConsulta(d);
    std::list<Registro> res;

    for (const Registro& r : rSub) {                /* O(n) */
        if (r[this->campo1()] == this->valor()) {   /* O(Len(c) + Equal(v)) */
            res.push_back(r);                       /* O(Len(c) + Copy(v))*/
        }
    }
    return res;
}

/** ================================================================================================================ **/
/**                                                   ..: MATCH :..                                                  **/
/** ================================================================================================================ **/

/* Descripción:
 * Complejidad:
 * Justificación:
 */
std::list<Registro> Consulta::procesarMatch(const BaseDeDatos& d) const {
    const NombreCampo& campo1 = this->campo1();
    const NombreCampo& campo2 = this->campo2();
    std::list<Registro> res;

    if (this->subconsulta1().tipo_consulta() == PRODUCT) {
        const Consulta& sub1 = this->subconsulta1().subconsulta1();
        const Consulta& sub2 = this->subconsulta1().subconsulta2();

        if (sub1.tipo_consulta() == FROM && sub2.tipo_consulta() == FROM) {
            NombreTabla nombreTabla1 = sub1.nombre_tabla();
            NombreTabla nombreTabla2 = sub2.nombre_tabla();
            const Tabla& t1 = d.obtenerTabla(nombreTabla1);
            const Tabla& t2 = d.obtenerTabla(nombreTabla2);

            if (nombreTabla1 != nombreTabla2 && campo1 == t1.clave() && campo2 == t2.clave()) {
                return join(t1, t2);
            }
        }
    }
    return procesarMatchBasico(d);
}

/* Descripción:
 * Complejidad:
 * Justificación:
 */
std::list<Registro> Consulta::join(const Tabla& t1, const Tabla& t2) const {
    const std::list<Registro>& rTabla1 = t1.registros();
    const std::list<Registro>& rTabla2 = t2.registros();
    std::list<Registro> res;

    if (rTabla1.size() < rTabla2.size()) {
        for (const Registro& r : rTabla1) {
            if (t2.regsValorEnCampo(this->campo2(), r[this->campo1()]).size() == 1) {
                const Registro& regClave =
                    t2.regsValorEnCampo(this->campo2(), r[this->campo1()]).front();
                res.push_back(pCartesiano(regClave, r));
            }
        }
    } else {
        for (const Registro& r : rTabla2) {
            if (t1.regsValorEnCampo(this->campo1(), r[this->campo2()]).size() == 1) {
                const Registro& regClave =
                    t1.regsValorEnCampo(this->campo1(), r[this->campo2()]).front();
                res.push_back(pCartesiano(regClave, r));
            }
        }
    }
    return res;
}

/* Descripción: Devuelve una copia de aquellos registros cuyos
 * campos especificados coinciden.
 * Complejidad: O(Q + n * (Len(c) + Equal(v) + Copy(v)))
 * Justificación: El costo de procesar una consulta de tipo match está
 * compuesto por el costo de procesar la subconsulta1, el costo de la
 * comparación entre dos valores, y el acceso al diccionario trie.
 * Estos costos están acotados por el acceso al campo de de mayor
 * longitud, 'c', y la comparación y el copiado del valor de mayor
 * tamaño, 'v'.
 */
std::list<Registro> Consulta::procesarMatchBasico(const BaseDeDatos& d) const {
    const std::list<Registro>& rSub = this->subconsulta1().procesarConsulta(d); /* O(Q) */
    std::list<Registro> res;

    for (const Registro& r : rSub) {
        if (r.def(this->campo1()) && r.def(this->campo2()) &&   /* O(Len(c)) */
            r[this->campo1()] == r[this->campo2()]) {           /* O(Len(c) + Equal(v)) */
            res.push_back(r);                                   /* O(Len(c) + Copy(v)) */
        }
    }
    return res;
}

/** ================================================================================================================ **/
/**                                                   ..: PROJ :..                                                   **/
/** ================================================================================================================ **/

/* Descripción: Devuelve una copia de la proyección del conjunto de
 * registros resultante de la subconsulta1.
 * Complejidad: O(Q + n * k * (Len(c) + Copy(c) + Copy(v)))
 * Justificación: Sea 'Q' el costo de procesar la subconsulta1 copiando
 * los registros resultantes. Tenemos que, para cada registro resultante
 * de la subconsulta1, 'n' en total, recorrer el conjunto de campos
 * (siendo 'k' el tamaño del mismo), e ir fijandonos si ese campo está
 * definido o no. El costo de esto último es O(Len(c)) donde 'c' es el
 * nombre del campo de mayor longitud. Si está definido, debemos copiarlo
 * en el registro proyectado, cuyo costo es O(Len(c) + Copy(v)) donde 'v'
 * es el valor de mayor tamaño.
 */
std::list<Registro> Consulta::procesarProj(const BaseDeDatos& d) const {
    const std::set<NombreCampo>& campos = this->conj_campos();                  /* O (k * Copy(c)) */
    const std::list<Registro>& rSub = this->subconsulta1().procesarConsulta(d); /* O(Q) */
    std::list<Registro> res;

    for (const Registro& r : rSub) {            /* O(n) */
        Registro tmp;

        for (const NombreCampo& c : campos) {   /* O(k) */
            if(r.def(c)) tmp.definir(c, r[c]);  /* O(Len(c) + Copy(v)) */
        }
        res.push_back(tmp);                     /* O(Len(c) + Copy(v)) */
    }
    return res;
}

/** ================================================================================================================ **/
/**                                                  ..: RENAME :..                                                  **/
/** ================================================================================================================ **/

/* Descripción: Devuelve por copia el conjunto de registros
 * resultantes de la subconsulta1 donde se renombró el
 * campo1 por campo2.
 * Complejidad: O(Q + n * (Equal(c) + Len(c) + Copy(v)))
 * Justificación: Si el costo de procesar la subconsulta1 es 'Q',
 * luego solo hay que considerar el costo de ir recorriendo el
 * conjunto de campos de cada registro, comparar si es el
 * campo a renombrar o no (O(Equal(c)), y en ambos casos realizar
 * una definición sobre el registro a devolver. Siendo el costo de
 * esto último O(Len(c) + Copy(v)) donde 'c' es el campo de mayor
 * longitud de todos los 'n' registros, y 'v' el valor de mayor tamaño.
 * Nota: El costo de la definición varía según si es el campo a
 * renombrar o no, en cuyo caso debería ser O(Len(c2) + Copy(v)), y
 * luego la complejidad del ciclo entero debería ser
 * O(Max(Len(c), Len(c2)) + Copy(v)). Para hacer los comentarios
 * mas legibles lo simplificamos en el valor que está.
 */
std::list<Registro> Consulta::procesarRename(const BaseDeDatos& d) const {
    const std::list<Registro>& rSub = (this->subconsulta1()).procesarConsulta(d); /* O(Q) */
    std::list<Registro> res;

    for (const Registro& r : rSub) {                    /* O(n) */
        Registro tmp;

        for (const NombreCampo& c : r.campos()) {       /* O(k) - k: Cte. */
            if (c == this->campo1()) {                  /* O(Equal(c)) */
                tmp.definir(this->campo2(), r[c]);      /* O(Len(c) + Copy(v)) */
            }
            else tmp.definir(c, r[c]);                  /* O(Len(c) + Copy(v)) */
        }
        res.push_back(tmp);                             /* O(Len(c) + Copy(v)) */
    }
    return res;
}

/** ================================================================================================================ **/
/**                                                   ..: INTER :..                                                  **/
/** ================================================================================================================ **/

/* Descripción: Devuelve el conjunto de registros en la intersección
 * de las dos subconsultas.
 * Complejidad: O(Q + n1 * n2 * k * (Len(c) + Copy(v)))
 * Justificación: Sean 'n1' y 'n2' la cantidad de registros resultantes
 * de las subconsultas, cuyo procesamiento cuesta O(Q), Entonces el
 * costo de devolver la intersección de estos conjuntos se reduce a
 * ver si dos registros son iguales. El costo de esta comparacioón es el
 * mismo al de comparar dos diccionarios trie, hay que recorrer las 'k'
 * claves, y viendo si al acceder a estas los valores son iguales.
 */
std::list<Registro> Consulta::procesarInter(const BaseDeDatos& d) const {
    const std::list<Registro>& rSub1 = (this->subconsulta1()).procesarConsulta(d);
    const std::list<Registro>& rSub2 = (this->subconsulta2()).procesarConsulta(d);
    std::list<Registro> res;

    for (const Registro& r1 : rSub1) {          /* O(n1) */
        for (const Registro& r2 : rSub2) {      /* O(n2) */
            if (r1 == r2) res.push_back(r1);    /* O(k * (Len(c) + Equal(v))) */
        }
    }
    return res;
}

/** ================================================================================================================ **/
/**                                                   ..: UNION :..                                                  **/
/** ================================================================================================================ **/

/* Descripción: Devuelve la union entre los registros resultantes de las
 * subconsultas especificadas.
 * Complejidad: O(Q + n * (Len(c) + Copy(v)))
 * Justificación: Sea 'Q' el costo máximo de procesar las subconsultas,
 * entonces si 'n' es la maxima
 */
std::list<Registro> Consulta::procesarUnion(const BaseDeDatos& d) const {
    const std::list<Registro>& rSub1 = (this->subconsulta1()).procesarConsulta(d); /* O(Q) */
    const std::list<Registro>& rSub2 = (this->subconsulta2()).procesarConsulta(d); /* O(Q) */
    std::list<Registro> res;

    for (const Registro& r1 : rSub1) {      /* O(n) */
        res.push_back(r1);                  /* O(Len(c) + Copy(v)) */
    }
    for (const Registro& r2 : rSub2) {      /* O(n) */
        res.push_back(r2);                  /* O(Len(c) + Copy(v) + k * (Len(c) + Equal(v))) */
    }
    res.unique();                           /* O(n) */
    return res;
}

/** ================================================================================================================ **/
/**                                                  ..: PRODUCT :..                                                 **/
/** ================================================================================================================ **/

/* Descripción:
 * Complejidad:
 * Justificación:
 */
std::list<Registro> Consulta::procesarProduct(const BaseDeDatos& d) const {
    const std::list<Registro>& rSub1 = this->subconsulta1().procesarConsulta(d);
    const std::list<Registro>& rSub2 = this->subconsulta2().procesarConsulta(d);
    std:list<Registro> res;

    for (const Registro& r1 : rSub1) {
        for (const Registro& r2 : rSub2) {
            res.push_back(pCartesiano(r1, r2));
        }
    }
    return res;
}

/* Descripción: Devuelve el registro resultante de hacer el producto
 * cartesiano entre 'r1' y 'r2'.
 * Complejidad: O(Len(c) + Copy(v))
 * Justificación: Como la cantidad de campos es acotada, recorrerlos
 * tiene costo constante. Luego la complejidad temporal en el peor
 * caso se reduce al acceso del campo más largo 'c' y el copiado del
 * valor más grande 'v'.
 */
Registro Consulta::pCartesiano(const Registro& r1, const Registro& r2) const{
    Registro reg = r1;                          /* O(Len(c) + Copy(v)) */

    for (const NombreCampo& c : r2.campos()) {  /* O(k) - k : Cte. */
        reg.definir(c, r2[c]);                  /* O(Len(c) + Copy(v)) */
    }
    return reg;
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