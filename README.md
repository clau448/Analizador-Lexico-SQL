# Analizador SQL: léxico y sintáctico LL(1)

Proyecto en C++11 del curso Estructuras de Datos Avanzadas. Incluye el analizador
léxico de la Práctica 4 y el parser predictivo de la Práctica 5.

Integrantes: Piero Adriano Cardenas Villagomez, Claudia Victoria Agostinelli
Cordova y Quinteros Condori Jesús Salvador.

El Lexer reconoce SELECT, FROM, WHERE, AND y OR sin distinguir mayúsculas,
identificadores, enteros, decimales, texto entre comillas simples, operadores
`=`, `<>`, `!=`, `<`, `>`, `<=`, `>=`, coma, punto y coma y asterisco. Conserva
lexema, línea y columna. El parser usa FIRST/FOLLOW, tabla LL(1) y pila explícita.

## Windows (PowerShell)

Instalar **CMake** y **Visual Studio 2022 / Build Tools 2022**, con la carga
**Desarrollo para el escritorio con C++** y un Windows SDK. Abrir una terminal
nueva para reconocer CMake y ejecutar desde la raíz del proyecto:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
.\build\Release\lexer.exe
.\build\Release\sql_parser.exe "SELECT nombre FROM alumnos WHERE edad >= 18;"
.\build\Release\sql_parser.exe --trace "SELECT * FROM alumnos;"
```

Si ya se configuró build con otro generador, usar una carpeta nueva como
build-msvc y sustituir build por build-msvc en todos los comandos.

También funciona con MinGW-w64 (g++ en PATH), sin CMake:

```powershell
g++ -std=c++11 main.cpp Lexer.cpp -o lexer.exe
g++ -std=c++11 parser_ll1/Grammar.cpp parser_ll1/Parser.cpp parser_ll1/main.cpp Lexer.cpp -o sql_parser.exe
.\lexer.exe
.\sql_parser.exe "SELECT * FROM alumnos;"
```

## macOS (Terminal)

Instalar las herramientas de compilación con `xcode-select --install` y CMake
(desde cmake.org o con `brew install cmake` si ya se utiliza Homebrew).
Desde la raíz:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
./build/lexer
./build/sql_parser "SELECT nombre FROM alumnos WHERE edad >= 18;"
./build/sql_parser --trace "SELECT * FROM alumnos;"
```

Sin CMake, usar el compilador de Apple:

```sh
clang++ -std=c++11 main.cpp Lexer.cpp -o lexer
clang++ -std=c++11 parser_ll1/Grammar.cpp parser_ll1/Parser.cpp parser_ll1/main.cpp Lexer.cpp -o sql_parser
./sql_parser "SELECT * FROM alumnos;"
```

## Linux y pruebas automatizadas

Con g++ y CMake instalados, usar los mismos comandos CMake de macOS.
Se conserva `make -C parser_ll1 test all` como alternativa para GNU Make.

CTest ejecuta las tres suites existentes, la demostración léxica y los casos de
consola: aceptación, rechazo y traza. Las comprobaciones assert permanecen
habilitadas en las pruebas incluso con Release. GitHub Actions compila y ejecuta
CTest en Windows (MSVC), macOS (AppleClang) y Linux (GCC); consultar la pestaña
Actions para los resultados reales de cada ejecución. Al pasar, cada trabajo
publica sus ejecutables como artefactos para su propio sistema operativo.
No trasladar un .exe de Windows a macOS ni un binario de macOS a Windows.

## Entrada, salida y alcance

`lexer` ejecuta tres ejemplos predefinidos, incluidos errores deliberados.
`sql_parser` recibe una consulta por argumentos o lee la entrada estándar si no
se le pasa una consulta. Imprime ACEPTADA y devuelve 0 al aceptar; devuelve 1 y
un diagnóstico con posición al rechazar. `--trace` muestra pila, entrada y acción.

El subconjunto sintáctico admite SELECT con * o lista de columnas, FROM con una
tabla, WHERE opcional y comparaciones unidas con AND/OR; exige punto y coma.
No incluye JOIN, paréntesis, comentarios, escapes de comillas ni números
negativos. No ejecuta consultas ni evalúa precedencia lógica o tipos de datos.

## Estructura y documentación

- Lexer.cpp, Lexer.h, Token.h y Keywords.h: reconocimiento léxico.
- main.cpp: demostración léxica con salida sin secuencias de color ANSI.
- parser_ll1/: gramática, parser, consola y pruebas.
- CMakeLists.txt: compilación común y CTest.
- [INFORME_ENTREGA.md](INFORME_ENTREGA.md): informe de la entrega inicial.
- [parser_ll1/README.md](parser_ll1/README.md): gramática y funcionamiento.
- [COLABORACION.md](COLABORACION.md): identidad Git y commits de cada integrante.

La extensión inicial del parser fue preparada con Codex mediante la cuenta de
Claudia. Los nombres de las ramas indican responsables de revisión; no cambian
la autoría. El historial conserva los aportes previos de los tres integrantes.
