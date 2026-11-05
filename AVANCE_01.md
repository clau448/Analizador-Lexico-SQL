# Primer avance - Practica de Laboratorio 5

## Objetivo

Construir un analizador sintactico predictivo LL(1), no recursivo, para un
subconjunto de consultas `SELECT`, integrado con el Lexer de la Practica 4.

## Arquitectura general

- `Lexer`: tokenizacion y posiciones, preservado desde la Practica 4.
- `Grammar`: producciones, FIRST, FOLLOW y tabla M[A,a].
- `Parser`: motor iterativo con pila explicita (rama progresiva de Piero).
- Diagnosticos, trazador y bateria ampliada (rama progresiva de Jesus).

## Gramatica propuesta

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

## FIRST y FOLLOW

| No terminal | FIRST | FOLLOW |
|---|---|---|
| Query | SELECT | $ |
| Projection | *, IDENTIFIER | FROM |
| ProjectionTail | comma, epsilon | FROM |
| WhereOpt | WHERE, epsilon | semicolon |
| Condition | IDENTIFIER | semicolon |
| ConditionTail | AND, OR, epsilon | semicolon |
| LogicOp | AND, OR | IDENTIFIER |
| Comparison | IDENTIFIER | AND, OR, semicolon |
| RelOp | =, <>, <, >, <=, >= | IDENTIFIER, INTEGER, DECIMAL, STRING |
| Value | IDENTIFIER, INTEGER, DECIMAL, STRING | AND, OR, semicolon |

## Tabla LL(1)

La tabla se construye desde los conjuntos de seleccion. Las entradas no vacias
son las producciones cuyo FIRST contiene el lookahead; para epsilon se usan las
columnas de FOLLOW. La prueba automatizada exige que la lista de conflictos sea
vacia, por lo que ninguna celda M[A,a] admite dos reglas.

## Estado actual real

Codex preparo el codigo de los tres modulos por encargo del equipo. No se
atribuye implementacion manual a ningun integrante. La rama de Claudia contiene
la base gramatical y este avance; las ramas de Piero y Jesus se construyen de
forma progresiva para que cada responsable revise, modifique y registre despues
su propia contribucion.

## Evidencia de compilacion y pruebas

La evidencia final se registra con los comandos y resultados realmente
ejecutados antes de publicar las ramas. El modulo gramatical se valida con:

```bash
make -C parser_ll1 test
```

## Distribucion de responsabilidades

- Claudia: revisar gramatica, FIRST/FOLLOW, tabla, documentacion y pruebas base.
- Piero: revisar motor no recursivo, pila e integracion con Lexer.
- Jesus: revisar errores, trazador, casos validos/invalidos y validacion final.

## Proximos pasos

Cada integrante debe revisar su rama, ejecutar las pruebas, realizar los ajustes
que considere necesarios con su propia autoria y aprobar la fusion progresiva.
