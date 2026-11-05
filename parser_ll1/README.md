# Analizador Sintactico Predictivo LL(1) SQL

La carpeta contiene la Practica 5 sin reemplazar el Lexer de la Practica 4. La
gramatica, sus conjuntos FIRST/FOLLOW y la tabla predictiva se construyen en
`Grammar.cpp`; la tabla se valida y reporta cualquier celda con mas de una
produccion.

## Gramatica

```text
Query          -> SELECT Projection FROM IDENTIFIER WhereOpt ;
Projection     -> * | IDENTIFIER ProjectionTail
ProjectionTail -> , IDENTIFIER ProjectionTail | epsilon
WhereOpt       -> WHERE Condition | epsilon
Condition      -> Comparison ConditionTail
ConditionTail  -> LogicOp Comparison ConditionTail | epsilon
LogicOp        -> AND | OR
Comparison     -> IDENTIFIER RelOp Value
RelOp          -> = | <> | < | > | <= | >=
Value          -> IDENTIFIER | INTEGER | DECIMAL | STRING
```

No existe recursion a la izquierda y las alternativas tienen conjuntos de
seleccion disjuntos. La implementacion requiere C++11.

## Verificacion del modulo gramatical

```bash
make -C parser_ll1 test
```

La prueba comprueba el numero de producciones, entradas representativas de
FIRST y FOLLOW, celdas de la tabla, ausencia de conflictos y el nuevo terminal
`*` integrado al Lexer existente.

## Compilacion y uso

```bash
make -C parser_ll1
./parser_ll1/sql_parser "SELECT nombre FROM alumnos WHERE edad >= 18;"
./parser_ll1/sql_parser --trace "SELECT * FROM alumnos;"
make -C parser_ll1 test
```

`Parser.cpp` implementa el algoritmo mediante `std::vector<Symbol>` usado como
pila. No llama recursivamente a ninguna funcion de analisis: consulta M[A,a],
expande la produccion en orden inverso o realiza `match` del terminal superior.

## Errores y traza

Ante una celda vacia, el parser informa linea, columna, lexema encontrado y los
terminales que tienen entrada en la fila de la tabla. Un error lexico se rechaza
sin acceder fuera de rango ni lanzar excepciones. `--trace` imprime, antes de
cada operacion, el numero de paso, la pila (tope a la izquierda), la entrada
restante y la produccion o emparejamiento aplicado.

La suite ejecuta casos simples y complejos, todos los operadores relacionales,
WHERE ausente/presente, AND/OR, errores lexicos y sintacticos, posicion en
varias lineas y contenido de la traza.

## Ramas y revision del equipo

| Rama | Responsable de revision | Contenido preparado por Codex |
|---|---|---|
| `feature/claudia-grammar` | Claudia Victoria Agostinelli Cordova | Gramatica, FIRST/FOLLOW, tabla y avance |
| `feature/piero-parser` | Piero Adriano Cardenas Villagomez | Pila iterativa e integracion con Lexer |
| `feature/jesus-tests` | Quinteros Condori Jesus Salvador | Errores, trazador y validacion completa |

Las ramas son progresivas en ese orden. Cada responsable debe revisar su modulo,
ejecutar `make -C parser_ll1 test` y registrar con su propia autoria cualquier
cambio que realice. El codigo inicial fue preparado por Codex; no se presenta
como implementacion manual de los integrantes.
