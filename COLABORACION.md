# Commits y contribuciones de Piero y Jesús

GitHub ya reconoce commits históricos de `pieroadrianoCV` y `YisusQc` en main.
Por ejemplo, Piero tiene `2fabe9d7` y Jesús tiene `806bbf62`.
Los commits iniciales del parser fueron preparados con Codex mediante la cuenta
`clau448`; una rama llamada feature/piero-parser o feature/jesus-tests no cambia
el autor. Fusionar un PR tampoco atribuye automáticamente su contenido a quien
hace el merge. No es necesario reescribir el historial.

## Cada integrante, desde su propio equipo y cuenta

1. Clonar el proyecto, o actualizar su copia sin descartar cambios locales:

```sh
git clone https://github.com/clau448/Analizador-Lexico-SQL.git
cd Analizador-Lexico-SQL
git switch main
git pull --ff-only origin main
```

Si ya existe la copia, omitir clone y entrar en ella. Si hay cambios pendientes,
revisarlos con `git status` antes de cambiar de rama.

2. Configurar la identidad local del repositorio (no la de otro integrante):

```sh
git config user.name "TU NOMBRE"
git config user.email "TU CORREO VERIFICADO EN GITHUB"
git var GIT_AUTHOR_IDENT
```

Reemplazar ambos valores. El correo debe estar asociado y verificado en su cuenta
GitHub (Settings → Emails), o ser exactamente el correo noreply que GitHub
muestra allí. No copiar un noreply inventado. El usuario autenticado para push
y el autor del commit son conceptos distintos; comprobar ambos.

3. Piero crea su rama con `git switch -c review/piero`; Jesús usa
`git switch -c review/jesus`. Cada uno revisa el código, ejecuta las pruebas y
realiza un cambio real. Ejemplos: Piero puede mejorar el tratamiento de una
entrada del parser; Jesús puede añadir casos de prueba con un resultado
esperado justificado. Si solo documentan su revisión, deben describir
honestamente qué revisaron y qué resultados observaron.

4. Registrar únicamente sus archivos modificados:

```sh
git diff
git add RUTA_DEL_ARCHIVO_MODIFICADO
git commit -m "test: describir el cambio realizado"
git log -1 --format=fuller
git push -u origin HEAD
```

Reemplazar la ruta y el mensaje por los del cambio real. Abrir un pull request
con destino main desde su propia cuenta. Integrarlo mediante merge conserva
los commits individuales; comprobar el resultado en la pestaña Commits.
No usar --author para asignarse código de otra persona ni crear commits vacíos
para aparentar aportes.

## Si no aparece una contribución

- Abrir el commit individual: la cuenta enlazada junto al autor permite verificar
  si GitHub reconoce su correo. Los ejemplos anteriores ya están vinculados.
- La lista de commits del repositorio, los autores de un archivo y el calendario
  del perfil son vistas distintas. Un archivo sin modificaciones propias no
  necesariamente mostrará su nombre.
- Para el calendario de contribuciones, el commit debe cumplir los requisitos de
  GitHub, entre ellos correo asociado y presencia en la rama predeterminada
  (o gh-pages); los aportes solo en una rama de trabajo pueden no contar aún.
- GitHub puede tardar hasta 24 horas en actualizar las contribuciones del perfil.
- Si un commit realmente suyo usa un correo antiguo, pueden asociar y verificar
  ese correo en su cuenta si les pertenece. Esto puede vincular commits existentes
  sin modificar sus hashes. No se ha comprobado la configuración privada de sus
  cuentas ni de sus computadoras.
