# Analizador Léxico SQL

Analizador léxico desarrollado en **C++** que procesa una cadena con una sentencia SQL y la divide en tokens. Para cada token identifica su categoría (tipo), conserva el lexema original y registra su posición mediante línea y columna.

> **Alcance actual:** el proyecto reconoce un subconjunto de elementos léxicos utilizados en sentencias `SELECT`. No valida la estructura gramatical completa de SQL ni ejecuta consultas contra una base de datos.

## Características implementadas

- Reconocimiento de palabras reservadas: `SELECT`, `FROM`, `WHERE`, `AND` y `OR`.
- Reconocimiento de palabras reservadas sin distinguir mayúsculas y minúsculas.
- Identificadores para nombres de tablas y columnas.
- Literales numéricos enteros y decimales.
- Literales de texto delimitados por comillas simples.
- Operadores relacionales: `=`, `!=`, `<>`, `<`, `<=`, `>` y `>=`.
- Signos de puntuación: coma (`,`) y punto y coma (`;`).
- Seguimiento de la posición de cada token (línea y columna).
- Identificación de fin de entrada (`TOKEN_EOF`) y tokens no reconocidos (`TOKEN_ERROR`).

## Tecnologías

- C++
- Biblioteca estándar de C++
- `std::string`, `std::vector` y `std::unordered_map`

## Estructura del proyecto

| Archivo | Responsabilidad |
|---|---|
| `Token.h` | Define `TokenType`, `Position` y la estructura `Token`. |
| `Keywords.h` | Mantiene el conjunto de palabras reservadas y permite reconocerlas sin distinguir mayúsculas y minúsculas. |
| `Lexer.h` | Declara la clase `Lexer`, sus atributos y sus métodos. |
| `Lexer.cpp` | Implementa el recorrido de la entrada y la identificación de los tokens. |
| `main.cpp` | Contiene consultas de demostración, ejecuta el análisis e imprime los resultados. |
| `.gitignore` | Excluye archivos compilados y temporales del control de versiones. |

## ¿Cómo funciona?

1. `main.cpp` define una cadena con una consulta SQL de prueba.
2. Se crea un objeto `Lexer` con esa cadena.
3. `tokenize()` solicita repetidamente el siguiente token mediante `getNextToken()`.
4. El analizador omite espacios en blanco, examina los caracteres y determina qué patrón reconoce.
5. Cada token almacena su tipo, lexema y posición.
6. El proceso termina al alcanzar `TOKEN_EOF`.
7. `main.cpp` muestra el lexema, tipo, línea y columna.

## Tipos de tokens

| Categoría | Tipos definidos |
|---|---|
| Palabras reservadas | `KEYWORD_SELECT`, `KEYWORD_FROM`, `KEYWORD_WHERE`, `KEYWORD_AND`, `KEYWORD_OR` |
| Identificadores | `IDENTIFIER` |
| Literales | `INTEGER_LITERAL`, `DECIMAL_LITERAL`, `STRING_LITERAL` |
| Operadores | `OP_EQUAL`, `OP_NOT_EQUAL`, `OP_LESS`, `OP_GREATER`, `OP_LESS_EQUAL`, `OP_GREATER_EQUAL` |
| Puntuación | `COMMA`, `SEMICOLON` |
| Control y errores | `TOKEN_EOF`, `TOKEN_ERROR` |

## Requisitos

- Un compilador de C++ (por ejemplo, `g++`).
- Una terminal compatible con la compilación de programas C++.

## Compilación y ejecución

Desde la carpeta raíz del repositorio:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp Lexer.cpp -o analizador
./analizador
```

En Windows, si el ejecutable generado tiene extensión `.exe`:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp Lexer.cpp -o analizador.exe
.\analizador.exe
```

El programa ejecuta las cadenas de prueba que están definidas actualmente en `main.cpp` y muestra los tokens en la terminal.

## Estado actual y limitaciones

La implementación cubre el reconocimiento léxico de un subconjunto de elementos SQL. Según el código presente en la rama `main`:

- La entrada de prueba está escrita directamente en `main.cpp`; todavía no se solicita una consulta al usuario ni se lee un archivo.
- No existe un analizador sintáctico que valide si los tokens forman una sentencia SQL correcta.
- No se ejecutan consultas contra un motor de base de datos.
- No se reconocen todavía muchas construcciones SQL, como `JOIN`, `ORDER BY`, `GROUP BY`, `INSERT`, `UPDATE`, `DELETE`, comentarios, paréntesis, asterisco ni operadores aritméticos.
- Las cadenas de texto no implementan secuencias de escape ni comillas simples escapadas.
- No se encontraron pruebas automatizadas, `Makefile` ni `CMakeLists.txt` en las rutas revisadas.

Los casos de demostración de `main.cpp` incluyen entradas mal formadas o caracteres no reconocidos, por lo que también permiten observar la emisión de `TOKEN_ERROR`.

## Mejoras recomendadas

1. Incorporar explícitamente `#include <cctype>` en `Keywords.h`, donde se utiliza `std::toupper`.
2. Añadir pruebas automatizadas para cada categoría de token y para entradas inválidas.
3. Separar los casos de demostración de las pruebas formales.
4. Permitir recibir consultas desde teclado o desde un archivo.
5. Ampliar gradualmente el conjunto de palabras reservadas y operadores según el alcance definido para el proyecto.
6. Incorporar un analizador sintáctico únicamente si el alcance del trabajo lo requiere.

## Alcance del proyecto

Este repositorio corresponde a un **analizador léxico**. Su responsabilidad es reconocer y clasificar unidades léxicas; no debe confundirse con un parser o analizador sintáctico. La documentación describe lo que está presente en el código revisado y distingue esas funcionalidades de las mejoras pendientes.
