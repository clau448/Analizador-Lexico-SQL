# Informe del proyecto: analizador léxico SQL y extensión sintáctica LL(1)

Fecha de revisión: 9 de noviembre de 2026 (UTC).
Curso: Estructuras de Datos Avanzadas.
Escuela Profesional de Ciencia de la Computación.

## Integrantes

- Piero Adriano Cardenas Villagomez.
- Claudia Victoria Agostinelli Cordova.
- Quinteros Condori Jesús Salvador.

## 1. Objetivo y alcance

El proyecto transforma consultas de un subconjunto de SQL en tokens y comprueba su estructura mediante un analizador predictivo LL(1). El módulo léxico corresponde a la Práctica 4 y la extensión sintáctica a la Práctica 5, según la documentación del repositorio. No se dispuso del enunciado original ni de la rúbrica de la profesora; por ello, la validación corresponde al alcance documentado en el proyecto.

El programa reconoce SELECT, FROM, WHERE, AND y OR sin distinguir mayúsculas de minúsculas; identificadores, enteros, decimales, cadenas entre comillas simples, operadores relacionales, coma, punto y coma y asterisco. Cada token conserva su lexema y su posición, con líneas y columnas desde 1.

La etapa sintáctica acepta proyección total o una lista de columnas, una tabla y WHERE opcional con comparaciones enlazadas mediante AND u OR. Exige punto y coma. No ejecuta consultas ni comprueba tablas, columnas o tipos de datos.

## 2. Arquitectura y funcionamiento

El flujo es: texto SQL → Lexer → vector de tokens → parser LL(1) → aceptación o diagnóstico.

- Token.h define los tipos de token y su posición.
- Keywords.h identifica las palabras reservadas.
- Lexer.h y Lexer.cpp recorren la entrada y reconocen lexemas.
- main.cpp muestra ejemplos léxicos, incluidos errores deliberados.
- parser_ll1/Grammar.cpp define 23 producciones, calcula FIRST/FOLLOW y construye la tabla predictiva.
- parser_ll1/Parser.cpp usa una pila explícita y consulta la tabla M[A,a], sin análisis recursivo.
- parser_ll1/main.cpp recibe consultas por argumentos o entrada estándar y ofrece --trace.
- parser_ll1/tests contiene pruebas gramaticales, sintácticas y de validación.

El Lexer omite espacios y actualiza línea y columna. Reconoce operadores de dos caracteres antes de devolver sus variantes simples. Un decimal exige dígitos después del punto; una cadena sin cerrar genera TOKEN_ERROR. Tanto != como <> se representan con OP_NOT_EQUAL y se normalizan al terminal <> del parser.

## 3. Gramática implementada

Query → SELECT Projection FROM IDENTIFIER WhereOpt ;
Projection → * | IDENTIFIER ProjectionTail
ProjectionTail → , IDENTIFIER ProjectionTail | ε
WhereOpt → WHERE Condition | ε
Condition → Comparison ConditionTail
ConditionTail → LogicOp Comparison ConditionTail | ε
LogicOp → AND | OR
Comparison → IDENTIFIER RelOp Value
RelOp → = | <> | < | > | <= | >=
Value → IDENTIFIER | INTEGER | DECIMAL | STRING

FIRST se calcula por iteración hasta alcanzar un punto fijo. FOLLOW propaga los posibles símbolos posteriores a cada no terminal. La tabla emplea FIRST del lado derecho y, para alternativas anulables, FOLLOW del lado izquierdo. Las pruebas verifican ausencia de conflictos; para esta gramática una celda no necesita elegir entre dos producciones.

El parser inicia con Query y el marcador $. Si el tope es un no terminal, aplica la producción de la tabla y coloca su lado derecho en orden inverso. Si es un terminal, lo compara con el token actual. Acepta cuando consume la entrada completa y vacía la pila. Ante el primer error devuelve línea, columna, lexema y, cuando corresponde, terminales esperados.

## 4. Compilación y demostración

Desde la raíz del proyecto:

```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp Lexer.cpp -o lexer
./lexer
make -C parser_ll1 test all
./parser_ll1/sql_parser "SELECT nombre FROM alumnos WHERE edad >= 18;"
./parser_ll1/sql_parser --trace "SELECT * FROM alumnos;"
```

También puede suministrarse un archivo mediante entrada estándar:

```bash
./parser_ll1/sql_parser < consulta.sql
```

Una consulta válida imprime ACEPTADA y devuelve código 0. Una consulta rechazada devuelve código 1 y un diagnóstico. Los ejemplos de errores incluidos en la demostración léxica son intencionales.

Ejemplo léxico: SELECT nombre FROM alumnos;
Produce SELECT, IDENTIFIER(nombre), FROM, IDENTIFIER(alumnos), ; y EOF.

Ejemplo de traza: SELECT * FROM alumnos;
La ejecución observada contiene nueve pasos: expansión de Query; match SELECT; expansión de Projection a *; match *; match FROM; match IDENTIFIER; expansión de WhereOpt a ε; match ; y match $. Resultado: ACEPTADA.

## 5. Verificación realizada

Se descargó el contenido exacto del commit 712c32efa6208bd3b339623e2b6da9d27833e4a8, que contiene las tres etapas progresivas, y se compiló con g++ en el entorno de revisión. No se modificó el código funcional para producir este informe.

- El analizador léxico compiló con C++11, -Wall, -Wextra y -pedantic, y ejecutó sus tres ejemplos.
- grammar_tests pasó: gramática, FIRST/FOLLOW, tabla y reconocimiento de *.
- parser_ll1_tests pasó: cuatro consultas válidas y dos inválidas.
- validation_tests pasó: diez consultas válidas y catorce inválidas, además de posición multilínea y contenido de la traza.
- El ejecutable del parser compiló y aceptó la demostración con --trace.

Las pruebas de validación cubren operadores relacionales, listas de columnas, WHERE opcional, AND/OR, falta de FROM, falta de punto y coma, condiciones incompletas y un carácter no permitido. Estas verificaciones no equivalen a implementar SQL completo.

## 6. Revisión del historial y explicación de las fusiones

Repositorio: https://github.com/clau448/Analizador-Lexico-SQL

Al iniciar la revisión, main apuntaba a f410506c5cbc1f2e0afbc5518d42a11be9e221f3. El PR #1 se había fusionado en main el 5 de noviembre. El PR #2 se había fusionado el 9 de noviembre a las 00:59:41 UTC, pero su destino era feature/claudia-grammar. Por ese motivo el motor de Piero todavía no aparecía en main. El PR #3 permanecía abierto y tenía como destino feature/piero-parser.

Los nombres de las ramas indican responsables previstos de revisión; por sí solos no prueban quién escribió el contenido. Los PR #1, #2 y #3 declaran expresamente que el código inicial de la extensión fue preparado por Codex. En esta revisión se preparó una rama de integración con todo el contenido progresivo para llevarlo directamente a main, conservando el historial existente.

Evidencias de aportes anteriores del módulo léxico:

- Piero: 6c574e62, “Añadiendo lexer y main”; 1ff3d232, “Refactoring”; 2fabe9d7, “Añadiendo el lexer.cpp faltante”, del 22 de octubre.
- Jesús (YisusQc): 275a2ba4, “feat: Add Token.h”, del 22 de octubre; 806bbf62, ampliación del alcance de tokens y aceptación de mayúsculas/minúsculas, del 1 de noviembre.
- Claudia (clau448): 32969c9e, commit inicial; e6fdbcd3 y a90b5382, documentación; 500bf9e8 y 6e3d41e0, ajustes de cctype; 29e15e29, base gramatical preparada con Codex.

Por tanto, sí existen commits atribuidos a los compañeros en el historial del analizador léxico. La ausencia de una notificación no demuestra ausencia de commits. No se verificaron las preferencias de notificación ni los equipos locales de los integrantes; tampoco se puede acreditar trabajo que no haya sido publicado. Los nombres y autores registrados en Git son evidencia del historial, no una certificación independiente de autoría manual.

## 7. Límites y mejoras

El subconjunto no incluye JOIN, alias, paréntesis, comentarios, funciones, expresiones aritméticas, números negativos ni cadenas con escape de comillas. Reconoce cadenas de AND/OR, pero no evalúa su significado ni calcula precedencia lógica. No genera un árbol de sintaxis ni resultados de una base de datos. Se detiene en el primer error sintáctico.

Antes de ampliar el lenguaje deben definirse nuevas reglas y pruebas. Para trabajo colaborativo futuro conviene dirigir los PR de entrega a main o fusionar explícitamente toda la cadena de ramas. Cada integrante debe registrar con su propia identidad los cambios que realmente realice.

## 8. Guion breve de exposición

1. Mostrar una consulta y explicar la diferencia entre lexema y token.
2. Ejecutar ./lexer y señalar las posiciones y los errores deliberados.
3. Presentar la gramática y explicar FIRST, FOLLOW y la tabla predictiva.
4. Ejecutar el parser con una consulta válida y otra sin punto y coma.
5. Activar --trace para mostrar la pila y las producciones.
6. Ejecutar make -C parser_ll1 test y explicar el alcance y las limitaciones.
7. Identificar los aportes comprobables y declarar la asistencia de Codex en la extensión.

## Referencias verificables

Repositorio: https://github.com/clau448/Analizador-Lexico-SQL
PR de gramática: https://github.com/clau448/Analizador-Lexico-SQL/pull/1
PR del motor: https://github.com/clau448/Analizador-Lexico-SQL/pull/2
PR de pruebas: https://github.com/clau448/Analizador-Lexico-SQL/pull/3
Versión probada: https://github.com/clau448/Analizador-Lexico-SQL/tree/712c32efa6208bd3b339623e2b6da9d27833e4a8
