# Informe tecnico - Practica de Laboratorio 5

## Alcance

Se implementa un subconjunto de `SELECT` con proyeccion total o explicita,
`FROM`, `WHERE` opcional, comparaciones y cadenas de condiciones enlazadas por
`AND`/`OR`. Toda sentencia termina en punto y coma.

## Propiedad LL(1)

El algoritmo calcula FIRST por punto fijo. FOLLOW se calcula propagando
`FIRST(beta) - {epsilon}` y, cuando `beta` es anulable, `FOLLOW(A)`. La tabla
M[A,a] se llena con FIRST de cada lado derecho y FOLLOW del lado izquierdo
para producciones anulables. Cualquier intento de ocupar una celda con otra
regla se conserva como conflicto; la suite exige cero conflictos.

Conjuntos principales:

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

La tabla completa se materializa programaticamente en `Grammar::parseTable()`;
cada celda guarda el identificador estable de una produccion.

## Integracion

El Lexer original se conserva y solamente se amplia con `STAR`, requerido por
`SELECT *`. Los demas tokens de la Practica 5 ya estaban presentes.
