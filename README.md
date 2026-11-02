# Analizador Léxico SQL

Analizador léxico desarrollado en **C++** para reconocer y clasificar los componentes de un subconjunto de sentencias SQL, tomando como referencia la instrucción `SELECT`. El programa recorre el texto de entrada, identifica los lexemas en el orden en que aparecen y genera los tokens correspondientes, registrando para cada uno su tipo y ubicación (línea y columna).

Este proyecto se desarrolla como parte de la **Práctica de Laboratorio 4: Analizador Léxico SQL**, del curso **Estructuras de Datos Avanzadas**, de la Escuela Profesional de Ciencia de la Computación.

## Integrantes

- Piero Adriano Cardenas Villagomez
- Claudia Victoria Agostinelli Cordova
- Integrante 3: ______________________________

## Objetivo

Implementar un analizador léxico capaz de transformar una cadena que contiene una sentencia SQL en una secuencia ordenada de tokens. Esta secuencia podrá utilizarse como entrada para una etapa posterior de análisis sintáctico.

El alcance de este módulo se limita al reconocimiento y clasificación de unidades léxicas. Por lo tanto, **no ejecuta consultas, no verifica la existencia de tablas o columnas y no valida la gramática completa de SQL**.

## Tokens reconocidos

El analizador identifica las siguientes categorías:

| Categoría | Lexemas o ejemplos |
|---|---|
| Palabras reservadas | `SELECT`, `FROM`, `WHERE`, `AND`, `OR` |
| Identificadores | Nombres de tablas y columnas, como `nombre`, `estudiantes` o `_temporal` |
| Literales enteros | `18`, `2026` |
| Literales decimales | `15.234`, `3.5` |
| Literales de texto | Cadenas delimitadas por comillas simples, como `'Arequipa'` |
| Operadores relacionales | `=`, `<>`, `!=`, `<`, `>`, `<=`, `>=` |
| Signos de puntuación | Coma (`,`) y punto y coma (`;`) |
| Fin de entrada | `TOKEN_EOF` |
| Error léxico | `TOKEN_ERROR`, para caracteres o secuencias que no cumplen las reglas reconocidas |

Las palabras reservadas se reconocen sin distinguir entre mayúsculas y minúsculas. Sin embargo, el lexema se conserva respetando la forma en que fue escrito en la entrada.

## Estructura del proyecto

El código fuente se organiza en los siguientes archivos:

| Archivo | Descripción |
|---|---|
| `main.cpp` | Contiene las consultas de demostración, inicia el análisis léxico y muestra los resultados en consola. |
| `Lexer.h` | Declara la clase `Lexer`, sus atributos y los métodos utilizados durante el análisis. |
| `Lexer.cpp` | Implementa el recorrido de los caracteres, el reconocimiento de lexemas y el seguimiento de las posiciones. |
| `Token.h` | Define los tipos de token, la estructura `Token` y la información de posición (línea y columna). |
| `Keywords.h` | Contiene el conjunto de palabras reservadas y la lógica para reconocerlas sin distinguir mayúsculas de minúsculas. |

## Requisitos

Para compilar y ejecutar el proyecto se necesita:

- Un compilador de C++ compatible con **C++11 o posterior**, como `g++` o `clang++`.
- Una terminal o consola desde la cual ejecutar los comandos de compilación.

## Compilación y ejecución

Ubícate en el directorio raíz del repositorio y ejecuta el siguiente comando:

```bash
g++ -std=c++11 main.cpp Lexer.cpp -o lexer
```

En Linux o macOS, inicia el programa con:

```bash
./lexer
```

En Windows, utilizando MinGW, genera el ejecutable y ejecútalo desde PowerShell:

```bash
g++ -std=c++11 main.cpp Lexer.cpp -o lexer.exe
.\lexer.exe
```

Al ejecutarse, el programa procesa las consultas de ejemplo definidas directamente en `main.cpp` y muestra los tokens identificados. La información presentada incluye:

```text
Lexema | Tipo de token | Línea | Columna
```

## Reglas léxicas implementadas

El reconocimiento de los componentes de la entrada se realiza aplicando las siguientes reglas:

- **Identificadores:** comienzan con una letra o guion bajo (`_`) y pueden continuar con letras, dígitos o guiones bajos.
- **Números enteros:** están formados por uno o más dígitos.
- **Números decimales:** contienen una parte entera, un punto y al menos un dígito en la parte decimal. Por ejemplo, `12.5` se reconoce como decimal, mientras que `12.` no cumple el formato esperado.
- **Cadenas de texto:** están delimitadas por comillas simples. No se permite que una cadena continúe en una línea diferente.
- **Operadores relacionales:** se reconocen tanto operadores de uno como de dos caracteres, por ejemplo, `=`, `>=`, `<=`, `<>` y `!=`.
- **Espacios en blanco:** los espacios, tabulaciones y saltos de línea se omiten durante el análisis, actualizando las posiciones correspondientes.
- **Posiciones:** el conteo de líneas y columnas comienza en `1`.
- **Errores léxicos:** los caracteres no reconocidos y las cadenas sin cerrar se clasifican como `TOKEN_ERROR`.

## Ejemplo de análisis

Considérese la siguiente sentencia:

```sql
SELECT nombre, edad FROM estudiantes WHERE edad >= 18;
```

Durante el análisis, el programa identifica los elementos de la sentencia en el orden en que aparecen:

1. `SELECT` — palabra reservada.
2. `nombre` — identificador.
3. `,` — coma.
4. `edad` — identificador.
5. `FROM` — palabra reservada.
6. `estudiantes` — identificador.
7. `WHERE` — palabra reservada.
8. `edad` — identificador.
9. `>=` — operador relacional.
10. `18` — literal entero.
11. `;` — punto y coma.
12. `TOKEN_EOF` — indica el final de la entrada.

## Alcance y estado actual

La implementación actual permite reconocer y clasificar los elementos léxicos descritos en esta documentación. Las consultas utilizadas para demostrar su funcionamiento se encuentran definidas directamente en `main.cpp`; por el momento, el programa no incorpora una interfaz para recibir sentencias desde la entrada estándar ni para leerlas desde un archivo.

Es importante señalar que el analizador puede generar tokens a partir de una secuencia que no constituya una sentencia SQL sintácticamente válida. Esto se debe a que su responsabilidad se limita a identificar lexemas y detectar errores de carácter léxico. La validación de la estructura de una consulta corresponde a una etapa posterior de análisis sintáctico.

Asimismo, este módulo no ejecuta consultas ni interactúa con un sistema gestor de bases de datos. Los ejemplos incluidos en `main.cpp` funcionan como demostraciones manuales; el repositorio todavía no cuenta con una suite independiente de pruebas automatizadas.

## Posibles mejoras

Como trabajo posterior, se consideran las siguientes mejoras:

- Incorporar la lectura de sentencias SQL desde teclado o desde archivos.
- Ampliar progresivamente el conjunto de palabras reservadas, operadores y símbolos reconocidos.
- Implementar pruebas automatizadas para los casos válidos y los errores léxicos.
- Mejorar el tratamiento de cadenas de texto, incluyendo posibles secuencias de escape.
- Desarrollar una etapa de análisis sintáctico que utilice la secuencia de tokens generada por este módulo.

---

**Nota:** Este proyecto aborda la primera etapa del procesamiento de una sentencia SQL: el análisis léxico. Su propósito es identificar y clasificar los componentes de la entrada, dejando la validación sintáctica y cualquier procesamiento posterior fuera del alcance de esta implementación.
