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
