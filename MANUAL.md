# 1. Présentation

Le projet [`CMake.Common.Modules`](https://dr-aix-git-01v.sccoa.si.c-s.fr/dlpx-pi/cmake.common.modules/) est un ensemble de modules CMake permettant une définition de projet simplifiée et automatisée.

Le projet nécessite le système de construction logicielle [CMake](https://cmake.org/) en version [3.14 ou supérieure](https://cmake.org/download/).

## 1.1. Module `Variables`

Le module [`Variables`](Variables.cmake) met à disposition un ensemble de fonctions relatives aux variables.

### Fonction `set_variable`

La fonction `set_variable` permet de définir une variable seulement si elle n'est pas préalablement définie.

```cmake
set_variable(<name> <value>)
```

| Nom     | Description |
|---------|-------------|
| `name`  | Nom         |
| `value` | Valeur      |

### Fonction `set_cache_variable`

La fonction `set_cache_variable` permet de définir une variable cache seulement si elle n'est pas préalablement définie.

```cmake
set_cache_variable(<name> <value> <type> <description>)
```

| Nom           | Description           |
|---------------|-----------------------|
| `name`        | Nom                   |
| `value`       | Valeur                |
| `type`        | Type                  |
| `description` | Description textuelle |

### Fonction `display_variables`

La fonction `display_variables` permet d'afficher dans la sortie standard CMake l'ensemble des variables du périmètre courant.

```cmake
display_variables()
```

### Fonction `display_matching_variables`

La fonction `display_matching_variables` permet d'afficher dans la sortie standard CMake l'ensemble des variables du périmètre courant correspondant au terme de recherche.

```cmake
display_matching_variables(<pattern>)
```

| Nom       | Description                                          |
|-----------|------------------------------------------------------|
| `pattern` | Terme de recherche _(texte ou expression régulière)_ |

## 1.2. Module `Common`

Le module [`Common`](Common.cmake) inclut tous les modules de base du projet `CMake.Common.Modules` et définit une valeur par défaut _(surchargeable)_ pour les variables relatives à l'espace de travail, au projet et à la construction.

### Définition de l'espace de travail

Les variables suivantes définissent l'espace de travail.

| Nom                          | Type   | Valeur                              | Description                   |
|------------------------------|--------|-------------------------------------|-------------------------------|
| `WORKSPACE_PATH`             | `PATH` | `${CMAKE_CURRENT_SOURCE_DIR}/..`    | Chemin de l'espace de travail |
| `SUBPROJECTS_PATH`           | `PATH` | `${CMAKE_BINARY_DIR}/..`            | Chemin des sous-projets       |
| `CMAKE_PREFIX_PATH`          | `PATH` | `${WORKSPACE_PATH}/external`        | Chemin des dépendances        |
| `CMAKE_MODULE_PATH`          | `PATH` | `${CMAKE_CURRENT_SOURCE_DIR}/cmake` | Chemin des modules            |
| `CMAKE_MODULE_LICENSE_PATH`  | `PATH` | `${CMAKE_MODULE_PATH}/license`      | Chemin des licences           |
| `CMAKE_MODULE_TEMPLATE_PATH` | `PATH` | `${CMAKE_MODULE_PATH}/templates`    | Chemin des modèles            |

### Définition du projet

Les variables suivantes définissent le projet.

| Nom                     | Type     | Valeur                        | Description           |
|-------------------------|----------|-------------------------------|-----------------------|
| `CMAKE_INSTALL_PREFIX`  | `PATH`   | `${CMAKE_BINARY_DIR}/install` | Chemin d'installation |
| `CMAKE_PROJECT_VERSION` | `STRING` | `0.0.0`                       | Version               |

Les options suivantes sont disponibles.

| Nom                         | Valeur  | Description                            |
|-----------------------------|---------|----------------------------------------|
| `BUILD_DOCUMENTATION`       | `FALSE` | Construction de la documentation       |
| `BUILD_TESTING`             | `TRUE`  | Construction des tests                 |
| `ENABLE_DOCUMENTATION_FULL` | `FALSE` | Documentation complète _(développeur)_ |
| `ENABLE_PDB_COPY`           | `FALSE` | Copie des fichiers PDB des dépendances |
| `ENABLE_PDB_INSTALL`        | `TRUE`  | Installation des fichiers PDB          |
| `ENABLE_PDB_RELEASE`        | `TRUE`  | Génération des fichiers PDB `RELEASE`  |

### Définition de la construction

Les variables suivantes définissent la construction.

| Nom                              | Type     | Valeur                              | Description                           |
|----------------------------------|----------|-------------------------------------|---------------------------------------|
| `TARGET_NAME`                    | `STRING` | `win-msvc2015-x64`                  | Architecture cible                    |
| `CMAKE_CXX_STANDARD`             | `NUMBER` | `11`                                | Version du langage C++                |
| `CMAKE_CXX_STANDARD_REQUIRED`    | `BOOL`   | `TRUE`                              | Norme standard C++                    |
| `CMAKE_CXX_EXTENSIONS`           | `BOOL`   | `FALSE`                             | Extensions C++                        |
| `CMAKE_ARCHIVE_OUTPUT_DIRECTORY` | `PATH`   | `${CMAKE_BINARY_DIR}/lib/$<CONFIG>` | Chemin des archives                   |
| `CMAKE_LIBRARY_OUTPUT_DIRECTORY` | `PATH`   | `${CMAKE_BINARY_DIR}/lib/$<CONFIG>` | Chemin des bibliothèques              |
| `CMAKE_RUNTIME_OUTPUT_DIRECTORY` | `PATH`   | `${CMAKE_BINARY_DIR}/bin/$<CONFIG>` | Chemin des exécutables                |
| `CMAKE_DEBUG_POSTFIX`            | `STRING` | `d`                                 | Suffixe des binaires `DEBUG`          |
| `CMAKE_MINSIZEREL_POSTFIX`       | `STRING` | `s`                                 | Suffixe des binaires `MINSIZEREL`     |
| `CMAKE_RELWITHDEBINFO_POSTFIX`   | `STRING` | `rd`                                | Suffixe des binaires `RELWITHDEBINFO` |
| `CMAKE_GENERATOR_PLATFORM`       | `STRING` | `x64`                               | Plateforme du générateur _(MSVC)_     |
| `CMAKE_CXX_FLAGS`                | `STRING` | _(voir ci-dessous)_                 | Options de compilation                |
| `CMAKE_CXX_FLAGS_DEBUG`          | `STRING` | _(voir ci-dessous)_                 | Options de compilation `DEBUG`        |
| `CMAKE_CXX_FLAGS_RELEASE`        | `STRING` | _(voir ci-dessous)_                 | Options de compilation `RELEASE`      |

Les options de compilation sont les suivantes.

| Compilateur | Nom                                                   | Valeur                                                                                  |
|-------------|-------------------------------------------------------|-----------------------------------------------------------------------------------------|
| MSVC        | `CMAKE_CXX_FLAGS`                                     | `/MP /W4 /wd4251 /wd4373`                                                               |
| GNU         | `CMAKE_CXX_FLAGS_DEBUG`<br/>`CMAKE_CXX_FLAGS_RELEASE` | `-Wall -Wextra -Wconversion -Wsign-conversion -Wold-style-cast`<br/>`-w -Wl,-s`         |
| Clang       | `CMAKE_CXX_FLAGS_DEBUG`<br/>`CMAKE_CXX_FLAGS_RELEASE` | `-Weverything -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic`<br/>`-w -Wl,-s` |

Les options suivantes sont disponibles.

| Nom                      | Valeur  | Description                         |
|--------------------------|---------|-------------------------------------|
| `ENABLE_CODE_COVERAGE`   | `FALSE` | Couverture de code _(GNU/Clang)_    |
| `ENABLE_CODE_GUIDELINES` | `FALSE` | Diagnostics de code _(GNU/Clang)_   |
| `ENABLE_CODE_SANITIZERS` | `FALSE` | Assainisseurs de code _(GNU/Clang)_ |

## 1.3. Module `Projects`

Le module [`Projects`](Projects.cmake) met à disposition un ensemble de fonctions relatives aux projets.

### Fonction `define_project`

La fonction `define_project` permet de définir un projet.

```cmake
define_project()
```

### Fonction `define_sub_project`

La fonction `define_sub_project` permet de définir un sous-projet _(dépendance reconstructible)_.

```cmake
define_sub_project(
    NAME <name>
    [MODE <mode>]
    [LOCAL_PATH <local_path>]
    [OUTPUT_PATH <output_path>]
    [REMOTE_PATH <remote_path>]
    [FIND_PATH <find_path>]
    [GIT_PATH <git_path>]
    [GIT_TAG <git_tag>]
    [SVN_PATH <svn_path>]
    [SVN_TAG <svn_tag>]
    [ARCHS <arch> ...]
    [TARGETS <target> ...]
    [ARGUMENTS <argument> ...])
```

| Nom           | Valeur (défaut)                                       | Description                               |
|---------------|-------------------------------------------------------|-------------------------------------------|
| `name`        |                                                       | Nom du sous-projet                        |
| `mode`        | `remote`                                              | Mode d'import _(local, remote, git, svn)_ |
| `local_path`  | `${WORKSPACE_PATH}/<name:lower>`                      | Chemin local                              |
| `output_path` | `${SUBPROJECTS_PATH}/<name:lower>`                    | Chemin local de l'adresse distante        |
| `remote_path` |                                                       | Adresse de l'artefact                     |
| `find_path`   | `${SUBPROJECTS_PATH}/<name:lower>/lib/${TARGET_NAME}` | Chemin de la définition CMake             |
| `git_path`    | `${GIT_URL}/<name:lower>.git`                         | Adresse du dépôt Git                      |
| `git_tag`     | `${<name:var>_VERSION}`                               | Branche, étiquette ou révision Git        |
| `svn_path`    | `${SUBVERSION_URL}/<name:lower>`                      | Adresse du dépôt Subversion               |
| `svn_tag`     | `${<name:var>_VERSION}`                               | Branche ou étiquette Subversion           |
| `arch`        |                                                       | Architecture de l'artefact                |
| `target`      |                                                       | Cible de l'artefact                       |
| `argument`    |                                                       | Argument de compilation                   |

La considération du sous-projet s'effectue depuis un chemin local ou une adresse distante _(artefact ou dépôt)_ selon le mode d'import sélectionné.

Les termes `%ARCH%` et `%TARGET%` du champ `<remote_path>` sont remplacés par chaque occurrence de `<arch>` et `<target>`.

### Fonction `define_component`

La fonction `define_component` permet de définir un composant _(interface, bibliothèque statique ou dynamique, exécutable)_.

```cmake
define_component(
    NAME <name>
    [PATH <path>]
    [EXECUTABLE]
    [STATIC]
    [INTERFACE]
    [RECURSIVE]
    [NOINSTALL]
    [INSTALLNAME <installname>]
    [INSTALLDIR <installdir>]
    [PUBLIC_BUILD_DEPS <public_build_dep> ...]
    [PRIVATE_BUILD_DEPS <private_build_dep> ...]
    [INTERFACE_BUILD_DEPS <interface_build_dep> ...]
    [RUNTIME_DEPS <runtime_dep> ...])
```

| Nom                   | Valeur (défaut)                                                  | Description                            |
|-----------------------|------------------------------------------------------------------|----------------------------------------|
| `name`                |                                                                  | Nom du composant                       |
| `path`                | `<name:.=/>`                                                     | Chemin des sources                     |
| `EXECUTABLE`          |                                                                  | Exécutable                             |
| `STATIC`              |                                                                  | Bibliothèque statique                  |
| `INTERFACE`           |                                                                  | Interface                              |
| `RECURSIVE`           |                                                                  | Recherche récursive des sources C++    |
| `NOINSTALL`           |                                                                  | Absence d'installation                 |
| `installname`         | `<name>`                                                         | Nom d'installation                     |
| `installdir`          | `bin/${TARGET_NAME}/$<CONFIG>`<br>`lib/${TARGET_NAME}/$<CONFIG>` | Répertoire d'installation              |
| `public_build_dep`    |                                                                  | Dépendance publique de construction    |
| `private_build_dep`   |                                                                  | Dépendance privée de construction      |
| `interface_build_dep` |                                                                  | Dépendance d'interface de construction |
| `runtime_dep`         |                                                                  | Dépendance d'exécution                 |

La construction du composant s'effectue selon sa définition.

Les dépendances publiques et d'exécution sont automatiquement copiées dans le répertoire de construction.

## 1.4. Module `Libraries`

Le module [`Libraries`](Libraries.cmake) met à disposition un ensemble de fonctions relatives aux bibliothèques.

### Fonction `find_package_libraries`

La fonction `find_package_libraries` permet de rechercher les bibliothèques d'un paquetage et d'en définir les variables associées.

```cmake
find_package_libraries(<name> <libnames> <include_path> <lib_path_debug> <lib_path_release>)
```

| Nom                | Description                   |
|--------------------|-------------------------------|
| `name`             | Nom du paquetage              |
| `libnames`         | Noms des bibliothèques        |
| `include_path`     | Chemin de l'entête principal  |
| `lib_path_debug`   | Chemin des binaires `DEBUG`   |
| `lib_path_release` | Chemin des binaires `RELEASE` |

Les variables `<name>_FOUND`, `<name>_DIR`, `<name>_INCLUDE_DIR` et `<name>_LIBRARIES` sont définies en sortie de l'appel de la fonction.

### Fonction `find_package_library`

La fonction `find_package_library` permet de rechercher une bibliothèque d'un paquetage et de l'ajouter aux cibles connues.

```cmake
find_package_library(<name> <libname> <include_path> <lib_path_debug> <lib_path_release>)
```

| Nom                | Description                   |
|--------------------|-------------------------------|
| `name`             | Nom du paquetage              |
| `libname`          | Nom de la bibliothèque        |
| `include_path`     | Chemin de l'entête principal  |
| `lib_path_debug`   | Chemin des binaires `DEBUG`   |
| `lib_path_release` | Chemin des binaires `RELEASE` |

La cible `<name>::<libname>` est définie et ajoutée aux cibles connues.

### Fonction `find_package_unique_libraries`

La fonction `find_package_unique_libraries` permet de rechercher les bibliothèques d'un paquetage et d'en définir les variables associées.

```cmake
find_package_unique_libraries(<name> <libnames> <include_path> <lib_path>)
```

| Nom            | Description                  |
|----------------|------------------------------|
| `name`         | Nom du paquetage             |
| `libnames`     | Noms des bibliothèques       |
| `include_path` | Chemin de l'entête principal |
| `lib_path`     | Chemin des binaires          |

Les variables `<name>_FOUND`, `<name>_DIR`, `<name>_INCLUDE_DIR` et `<name>_LIBRARIES` sont définies en sortie de l'appel de la fonction.

### Fonction `find_package_unique_library`

La fonction `find_package_unique_library` permet de rechercher une bibliothèque d'un paquetage et de l'ajouter aux cibles connues.

```cmake
find_package_unique_library(<name> <libname> <include_path> <lib_path>)
```

| Nom            | Description                  |
|----------------|------------------------------|
| `name`         | Nom du paquetage             |
| `libname`      | Nom de la bibliothèque       |
| `include_path` | Chemin de l'entête principal |
| `lib_path`     | Chemin des binaires          |

La cible `<name>::<libname>` est définie et ajoutée aux cibles connues.

## 1.5. Module `Dependencies`

Le module [`Dependencies`](Dependencies.cmake) met à disposition un ensemble de fonctions relatives aux dépendances.

### Fonction `define_dependency`

La fonction `define_dependency` permet de récupérer et définir une dépendance.

```cmake
define_dependency(
    NAME <name>
    [DIRNAME <dirname>]
    [OUTPUT_PATH <output_path>]
    REMOTE_PATH <remote_path>
    [FIND_PATH <find_path>]
    [CREATEDIR]
    [NOFIND]
    [CMAKE]
    [GIT]
    [SVN]
    [ARCHS <arch> ...]
    [TARGETS <target> ...]
    [COMPONENTS <component> ...])
```

| Nom           | Valeur (défaut)           | Description                        |
|---------------|---------------------------|------------------------------------|
| `name`        |                           | Nom de la dépendance               |
| `dirname`     | `<name>`                  | Nom du répertoire local            |
| `output_path` | `${CMAKE_PREFIX_PATH}`    | Chemin local de l'adresse distante |
| `remote_path` |                           | Adresse de l'artefact ou du dépôt  |
| `find_path`   | `<output_path>/<dirname>` | Chemin de la définition CMake      |
| `CREATEDIR`   |                           | Création du répertoire `<dirname>` |
| `NOFIND`      |                           | Absence de recherche de paquetage  |
| `CMAKE`       |                           | Paquetage CMake                    |
| `GIT`         |                           | Dépôt Git                          |
| `SVN`         |                           | Dépôt Subversion                   |
| `arch`        | `${DD_ARCHS}`             | Architecture de l'artefact         |
| `target`      | `${DD_TARGETS}`           | Cible de l'artefact                |
| `component`   |                           | Composant du paquetage             |

La récupération, l'actualisation et la considération de la dépendance s'effectue depuis l'adresse distante _(artefact ou dépôt)_.

Les termes `%ARCH%` et `%TARGET%` du champ `<remote_path>` sont remplacés par chaque occurrence de `<arch>` et `<target>`.

### Fonction `download_dependency`

La fonction `download_dependency` permet de récupérer une dépendance.

```cmake
download_dependency(
    DIRNAME <dirname>
    OUTPUT_PATH <output_path>
    REMOTE_PATH <remote_path>
    [CREATEDIR]
    [GIT]
    [SVN]
    [ARCHS <arch> ...]
    [TARGETS <target> ...])
```

| Nom           | Valeur (défaut)           | Description                        |
|---------------|---------------------------|------------------------------------|
| `dirname`     |                           | Nom du répertoire local            |
| `output_path` |                           | Chemin local de l'adresse distante |
| `remote_path` |                           | Adresse de l'artefact ou du dépôt  |
| `CREATEDIR`   |                           | Création du répertoire `<dirname>` |
| `GIT`         |                           | Dépôt Git                          |
| `SVN`         |                           | Dépôt Subversion                   |
| `arch`        | `${DD_ARCHS}`             | Architecture de l'artefact         |
| `target`      | `${DD_TARGETS}`           | Cible de l'artefact                |

La récupération et l'actualisation de la dépendance s'effectue depuis l'adresse distante _(artefact ou dépôt)_.

Les termes `%ARCH%` et `%TARGET%` du champ `<remote_path>` sont remplacés par chaque occurrence de `<arch>` et `<target>`.

### Fonction `get_runtime_dependencies`

La fonction `get_runtime_dependencies` permet de rechercher récursivement les dépendances d'exécution d'un composant.

```cmake
get_runtime_dependencies(<name>)
```

| Nom    | Description      |
|--------|------------------|
| `name` | Nom du composant |

La variable `RUNTIME_DEPS` est définie en sortie de l'appel de la fonction.

## 1.6. Module `Tests`

Le module [`Tests`](Tests.cmake) met à disposition un ensemble de fonctions relatives aux tests.

### Fonction `define_tests`

La fonction `define_tests` permet de définir les tests d'un projet.

```cmake
define_tests()
```

## 1.7. Module `Documentations`

Le module [`Documentations`](Documentations.cmake) met à disposition un ensemble de fonctions relatives aux documentations.

### Fonction `define_documentation`

La fonction `define_documentation` permet de définir la documentation d'un projet.

```cmake
define_documentation()
```

## 1.8. Module `Installations`

Le module [`Installations`](Installations.cmake) met à disposition un ensemble de fonctions relatives aux installations.

### Fonction `define_installation`

La fonction `define_installation` permet de définir l'installation d'un projet.

```cmake
define_installation()
```

### Fonction `install_runtime_dependencies`

La fonction `install_runtime_dependencies` permet de définir l'installation des dépendances d'exécution d'un composant.

```cmake
install_runtime_dependencies(<name> <installname> <installdir>)
```

| Nom           | Description               |
|---------------|---------------------------|
| `name`        | Nom du composant          |
| `installname` | Nom d'installation        |
| `installdir`  | Répertoire d'installation |

## 1.9. Module `Packages`

Le module [`Packages`](Packages.cmake) met à disposition un ensemble de fonctions relatives aux paquetages.

### Fonction `define_package`

La fonction `define_package` permet de définir le paquetage d'un projet.

```cmake
define_package()
```

## 1.10. Module `Authentications`

Le module [`Authentications`](Authentications.cmake) met à disposition un ensemble de fonctions relatives aux identifiants.

### Fonction `get_authentication`

La fonction `get_authentication` permet de récupérer les identifiants depuis les variables d'environnement ou un module `Logins`.

```cmake
get_authentication(<name>)
```

| Nom           | Description                                  |
|---------------|----------------------------------------------|
| `name`        | Nom de l'identifiant (Git, Subversion, etc.) |

Les variables `<name>_USERNAME` et `<name>_PASSWORD` sont définies en sortie de l'appel de la fonction.

## 1.11. Module `Logins`

Le module [`Logins`](Logins.cmake) est un cartouche permettant de définir les identifiants.

```cmake
set(GIT_USERNAME "<git_username>")
set(GIT_PASSWORD "<git_password>")
set(SVN_USERNAME "<svn_username>")
set(SVN_PASSWORD "<svn_password>")
```

| Nom            | Description             |
|----------------|-------------------------|
| `git_username` | Identifiant Git         |
| `git_password` | Mot de passe Git        |
| `svn_username` | Identifiant Subversion  |
| `svn_password` | Mot de passe Subversion |

## 1.12. Module `Tools`

Le module [`Tools`](Tools.cmake) met à disposition un ensemble de fonctions relatives aux outils _(cibles personnalisées)_.

# 2. Utilisation

Cette section présente l'ensemble des étapes permettant une définition de projet simplifiée et automatisée en utilisant les fonctionnalités du projet `CMake.Common.Modules`.

## 2.1. Intégration des modules

Il convient d'ajouter le projet `CMake.Common.Modules` comme sous-module du dépôt Git du projet.

```bash
git submodule add https://dr-aix-git-01v.sccoa.si.c-s.fr/dlpx-pi/cmake.common.modules.git cmake
git submodule update --init
```

Il est possible d'actualiser la révision du sous-module afin de disposer des dernières fonctionnalités.

```bash
git submodule update --remote
```

## 2.2. Édition des modules

Il convient de créer une nouvelle branche ou d'effectuer une divergence du projet `CMake.Common.Modules` afin de pouvoir en éditer les modules et correspondre aux besoins propres d'un projet.

L'intégration éventuelle de ces modifications dans la branche principale est soumise aux responsables du projet `CMake.Common.Modules`.

## 2.3. Définition d'un module commun

Il est fortement conseillé de définir un module commun `Common*.cmake` propre à un ensemble de projets.

```cmake
# Workspace definition and options
# Note: Ajouter les variables communes et relatives à la spécialisation de l'espace de travail

# Project definition and options
# Note: Ajouter les variables communes et relatives à la spécialisation de l'ensemble de projets

# Compilation definition and options
# Note: Ajouter les variables communes et relatives à la spécialisation de la construction

# Common includes
include(Common)
# Note: Ajouter les modules communs et relatifs à l'ensemble de projets

# Ajouter les fonctions communes et relatives à l'ensemble de projets
```

Le module commun [`CommonDlpxPi`](CommonDlpxPi.cmake) de l'ensemble de projets _DLPX-PI_ est disponible comme exemple.

## 2.4. Définition d'un projet

Il convient de définir le projet dans le fichier principal `CMakeLists.txt` associé au projet.

```cmake
cmake_minimum_required(VERSION 3.14)

# Note: Nécessaire pour initialiser correctement les variables des modules
project(FakeProject)

# Note: Surcharger si nécessaire les variables cache des modules communs
# set(VARIABLE_NAME "VARIABLE_VALUE" CACHE STRING "Variable description")

# Définition des modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Common)  # Note: Remplacer si nécessaire par un autre module commun

# Définition du projet
project(MyProject VERSION ${CMAKE_PROJECT_VERSION})
define_project()
```

## 2.5. Définition d'un composant

Il convient de définir un composant dans le fichier `CMakeLists.txt` associé au composant.

```cmake
define_component(
    NAME "MyProject.MyComponent")

# Note: Rectifier si nécessaire le chemin PATH
# Note: Spécifier si la recherche des sources est RECURSIVE
# Note: Spécifier les dépendances PUBLIC_BUILD_DEPS, PRIVATE_BUILD_DEPS et INTERFACE_BUILD_DEPS
```

### Définition d'une interface

La déclaration suivante définit une interface.

```cmake
define_component(
    NAME "MyProject.MyInterface"
    INTERFACE)

# Note: Rectifier si nécessaire le chemin PATH
# Note: Spécifier si la recherche des sources est RECURSIVE
```

### Définition d'une bibliothèque statique

La déclaration suivante définit une bibliothèque statique.

```cmake
define_component(
    NAME "MyProject.MyStaticLibrary"
    STATIC)

# Note: Rectifier si nécessaire le chemin PATH
# Note: Spécifier si la recherche des sources est RECURSIVE
# Note: Spécifier les dépendances PUBLIC_BUILD_DEPS, PRIVATE_BUILD_DEPS et INTERFACE_BUILD_DEPS
```

### Définition d'une bibliothèque dynamique

La déclaration suivante définit une bibliothèque dynamique.

```cmake
define_component(
    NAME "MyProject.MySharedLibrary")

# Note: Rectifier si nécessaire le chemin PATH
# Note: Spécifier si la recherche des sources est RECURSIVE
# Note: Spécifier les dépendances PUBLIC_BUILD_DEPS, PRIVATE_BUILD_DEPS et INTERFACE_BUILD_DEPS
```

### Définition d'un exécutable

La déclaration suivante définit un exécutable.

```cmake
define_component(
    NAME "MyProject.MyExecutable"
    EXECUTABLE)

install_runtime_dependencies("MyProject.MyExecutable")

# Note: Rectifier si nécessaire le chemin PATH
# Note: Spécifier si la recherche des sources est RECURSIVE
# Note: Spécifier les dépendances PUBLIC_BUILD_DEPS, PRIVATE_BUILD_DEPS, INTERFACE_BUILD_DEPS et RUNTIME_DEPS
```

### Définition d'un test

La déclaration suivante définit un test.

```cmake
define_component(
    NAME "MyProject.MyTest"
    EXECUTABLE
    NOINSTALL)

gtest_discover_tests("MyProject.MyTest" WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Note: Rectifier si nécessaire le chemin PATH
# Note: Spécifier si la recherche des sources est RECURSIVE
# Note: Spécifier les dépendances PUBLIC_BUILD_DEPS, PRIVATE_BUILD_DEPS, INTERFACE_BUILD_DEPS et RUNTIME_DEPS
```

## 2.6. Définition d'un sous-projet

Il convient de définir un sous-projet de préférence dans un module commun `Common*.cmake` _(plutôt que dans le fichier principal `CMakeLists.txt`)_ afin de le rendre accessible pour l'ensemble de projets.

```cmake
# Définition unitaire
define_sub_project(
    NAME "${MySubProject_NAME}"
    REMOTE_PATH "${ARTIFACTORY_URL}/${ARTIFACTORY_DIR}/${MySubProject_NAME}/${MySubProject_VERSION}/${MySubProject_NAME}.${MySubProject_VERSION}.zip")

# Définition commune
function(get_sub_project_${MyProjects} NAME)
    if(NAME STREQUAL "${MySubProject_NAME}")
        define_sub_project(
            NAME "${MySubProject_NAME}"
            REMOTE_PATH "${ARTIFACTORY_URL}/${ARTIFACTORY_DIR}/${MySubProject_NAME}/${MySubProject_VERSION}/${MySubProject_NAME}.${MySubProject_VERSION}.zip")
    endif(NAME STREQUAL "${MySubProject_NAME}")
    # Note: Ajouter les autres sous-projets
endfunction(get_sub_project_${Projects} NAME)

# Utilisation de la définition commune
get_sub_project_${MyProjects}(${MySubProject_NAME})

# Note: Rectifier si nécessaire les chemins LOCAL_PATH, OUTPUT_PATH, FIND_PATH, GIT_PATH et SVN_PATH
```

## 2.7. Définition d'une dépendance

Il convient de définir une dépendance de préférence dans un module commun `Common*.cmake` _(plutôt que dans le fichier principal `CMakeLists.txt`)_ afin de la rendre accessible pour l'ensemble de projets.

```cmake
# Définition unitaire
define_dependency(
    NAME "${MyDependency_NAME}"
    REMOTE_PATH "${ARTIFACTORY_URL}/${ARTIFACTORY_DIR}/${MyDependency_NAME}/${MyDependency_VERSION}/${MyDependency_NAME}.${MyDependency_VERSION}.zip")

# Définition commune
function(get_dependency_${MyProjects} NAME)
    if(NAME STREQUAL "${MyDependency_NAME}")
        define_dependency(
            NAME "${MyDependency_NAME}"
            REMOTE_PATH "${ARTIFACTORY_URL}/${ARTIFACTORY_DIR}/${MyDependency_NAME}/${MyDependency_VERSION}/${MyDependency_NAME}.${MyDependency_VERSION}.zip")
    endif(NAME STREQUAL "${MyDependency_NAME}")
    # Note: Ajouter les autres dépendances
endfunction(get_dependency_${Projects} NAME)

# Utilisation de la définition commune
get_dependency_${MyProjects}(${MyDependency_NAME})

# Note: Rectifier si nécessaire le nom DIRNAME
# Note: Rectifier si nécessaire les chemins OUTPUT_PATH et FIND_PATH
# Note: Spécifier si nécessaire CREATEDIR, NOFIND, CMAKE, GIT et SVN
```

### Dépendance compatible CMake

Une dépendance compatible CMake _(incluant une définition `*.cmake`)_ est nativement considérée après analyse du chemin `FIND_PATH`.

```cmake
define_dependency(
    NAME "${MyDependency_NAME}"
    ...
    CMAKE)
```

L'installation d'un projet défini en utilisant les fonctionnalités du projet `CMake.Common.Modules` est compatible CMake.

### Dépendance incompatible CMake

Une dépendance incompatible CMake _(n'incluant pas de définition `*.cmake`)_ n'est pas nativement considérée.

```cmake
define_dependency(
    NAME "${MyDependency_NAME}"
    ...)
```

Il convient de définir un module `Find${MyDependency_NAME}.cmake` afin de permettre sa compatibilité.

```cmake
find_package_libraries(
  "${MyDependency_NAME}"
  "${MyDependency_LIBNAME1};${MyDependency_LIBNAME2};..."
  "${MyDependency_INCLUDE_PATH}"
  "${MyDependency_LIB_PATH_DEBUG}"
  "${MyDependency_LIB_PATH_RELEASE}")
```

Dans l'éventualité où la fonction `find_package_libraries` ne permet pas de définir sa compatibilité, il convient de définir un module `Libraries${MyDependency_NAME}.cmake` implémentant les fonctions `find_${MyDependency_NAME}_package_libraries` et `find_${MyDependency_NAME}_package_library` pour les besoins spécifiques de la dépendance.

Le module `Libraries${MyDependency_NAME}.cmake` est ensuite ajouté à un module commun `Common*.cmake` ou au fichier principal  `CMakeLists.txt`.

```cmake
include(Libraries${MyDependency_NAME})
```

## 2.8. Définition des tests

Il convient de définir les tests dans le fichier principal `CMakeLists.txt`.

```cmake
# Note: Positionner après l'appel de la fonction define_project()
define_tests()
enable_testing()
```

Les tests nécéssitent la dépendance `GoogleTest` ou sa présence préalable sur le système et dans l'environnement.

## 2.9. Définition de la documentation

Il convient de définir la documentation dans le fichier principal `CMakeLists.txt`.

```cmake
# Note: Positionner après l'appel de la fonction define_project()
define_documentation()
```

La documentation nécéssite la dépendance `Doxygen` ou sa présence préalable sur le système et dans l'environnement.

## 2.10. Définition de l'installation

Il convient de définir l'installation dans le fichier principal `CMakeLists.txt`.

```cmake
# Note: Positionner après l'appel de la fonction define_project()
define_installation()
```

## 2.11. Définition du paquetage

Il convient de définir le paquetage dans le fichier principal `CMakeLists.txt`.

```cmake
# Note: Positionner après l'appel de la fonction define_project()
define_package()
```

La documentation nécéssite la dépendance `NSIS` ou sa présence préalable sur le système et dans l'environnement.

### Commande d'installation

Il convient de définir une commande d'installation avant l'appel de la fonction `define_package`.

La déclaration suivante définit l'ajout des raccourcis vers un exécutable dans le menu `Démarrer` et sur le bureau.

```cmake
list(APPEND CPACK_NSIS_CREATE_ICONS_EXTRA "IntCmp $MyExecutable_selected 0 addshortcut_MyExecutable")
list(APPEND CPACK_NSIS_CREATE_ICONS_EXTRA "  CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\MyExecutable.lnk' '$INSTDIR\\\\MyProject.MyExecutable.exe'")
list(APPEND CPACK_NSIS_CREATE_ICONS_EXTRA "  CreateShortCut '$DESKTOP\\\\MyExecutable.lnk' '$INSTDIR\\\\MyProject.MyExecutable.exe'")
list(APPEND CPACK_NSIS_CREATE_ICONS_EXTRA "addshortcut_MyExecutable:")
```

La déclaration suivante définit l'exécution d'un script batch.

```cmake
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "SetOutPath $INSTDIR")
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExpandEnvStrings $0 %COMSPEC%")
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$0\\\" /C \\\"$INSTDIR\\\\MyInstallBatchScript.bat\\\"'")
```

### Commande de désinstallation

Il convient de définir une commande de désinstallation avant l'appel de la fonction `define_package`.

La déclaration suivante définit la suppression des raccourcis vers un exécutable dans le menu `Démarrer` et sur le bureau.

```cmake
list(APPEND CPACK_NSIS_DELETE_ICONS_EXTRA "Delete '$SMPROGRAMS\\\\$START_MENU\\\\MyExecutable.lnk'")
list(APPEND CPACK_NSIS_DELETE_ICONS_EXTRA "Delete '$DESKTOP\\\\MyExecutable.lnk'")
```

La déclaration suivante définit l'exécution d'un script batch.

```cmake
list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "SetOutPath $INSTDIR")
list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "ExpandEnvStrings $0 %COMSPEC%")
list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "ExecWait '\\\"$0\\\" /C \\\"$INSTDIR\\\\MyUninstallBatchScript.bat\\\"'")
```

## 2.12. Définition d'un outil

Il convient de définir un outil de préférence dans un module outils `Tools*.cmake` _(plutôt que dans le fichier principal `CMakeLists.txt`)_ afin de le rendre accessible pour l'ensemble de projets.

```cmake
function(${MyTool_NAME} ...)
    ...
endfunction(${MyTool_NAME} ...)
```

Le module outils est ensuite ajouté à un module commun `Common*.cmake` ou au fichier principal  `CMakeLists.txt`.

```cmake
include(${MyToolsModule})
```

Les outils peuvent ensuite être exécutés en tant que cibles et intégrés à l'environnement de développement.

### Exemple d'outil shell

La déclaration suivante définit un outil exécutant un script shell avec deux arguments.

```cmake
function(MyShellScript)
    set(GitBash_EXECUTABLE "C:/Program Files/Git/bin/bash.exe")
    if(EXISTS ${GitBash_EXECUTABLE})
        set(MyShellScript_PATH "${MyShellScript_DIR}/MyShellScript.sh" CACHE INTERNAL "MyShellScript path")
        set(MyShellScript_ARG1 "${MyShellScript_ARG1_value}" CACHE STRING "MyShellScript argument #1")
        set(MyShellScript_ARG2 "${MyShellScript_ARG2_value}" CACHE STRING "MyShellScript argument #2")
        add_custom_target("MyTools.MyShellScript")
        add_custom_command(TARGET "MyTools.MyShellScript"
            COMMAND "${GitBash_EXECUTABLE} ${MyShellScript_PATH} ${MyShellScript_ARG1} ${MyShellScript_ARG2}")
    endif(EXISTS ${GitBash_EXECUTABLE})
endfunction(MyShellScript)
```

### Exemple d'outil Python

La déclaration suivante définit un outil exécutant un script Python avec deux arguments.

```cmake
function(MyPythonScript)
    find_package(Python 3.7.3 COMPONENTS Interpreter)
    if(Python_FOUND)
        set(MyPythonScript_PATH "${MyPythonScript_DIR}/MyPythonScript.py" CACHE INTERNAL "MyPythonScript path")
        set(MyPythonScript_ARG1 "${MyPythonScript_ARG1_value}" CACHE STRING "MyPythonScript argument #1")
        set(MyPythonScript_ARG2 "${MyPythonScript_ARG2_value}" CACHE STRING "MyPythonScript argument #2")
        add_custom_target("MyTools.MyPythonScript")
        add_custom_command(TARGET "MyTools.MyPythonScript"
            COMMAND "${Python_EXECUTABLE} ${MyPythonScript_PATH} ${MyPythonScript_ARG1} ${MyPythonScript_ARG2}")
    endif(Python_FOUND)
endfunction(MyPythonScript)
```

## 2.13. Configuration du projet

Il convient de configurer le projet en ligne de commande ou via l'application [`cmake-gui`](https://cmake.org/cmake/help/latest/manual/cmake-gui.1.html).

```bash
cmake -S ${SOURCE_DIR} -B ${BUILD_DIR} ...
```

### Surcharge des variables

La surcharge des variables cache est possible _(initialement et ultérieurement)_ en ligne ou commande ou via l'application `cmake-gui`.

```bash
cmake -S ${SOURCE_DIR} -B ${BUILD_DIR} -D${MyVariable_NAME}=${MyVariable_VALUE} ...
```

### Sélection du compilateur

La sélection du compilateur est possible _(initialement)_ en ligne de commande.

```bash
# Sélection directe
cmake -S ${SOURCE_DIR} -B ${BUILD_DIR} -DCMAKE_C_COMPILER=${CCompilerName} -DCMAKE_CXX_COMPILER=${CxxCompilerName} ...

# Sélection indirecte
export C_COMPILER=${CCompilerName}
export CXX_COMPILER=${CxxCompilerName}
cmake -S ${SOURCE_DIR} -B ${BUILD_DIR} ...

# Cas particulier MSVC
call "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin/amd64/vcvars64.bat"
cmake -S ${SOURCE_DIR} -B ${BUILD_DIR} ...
```

### Sélection de la chaîne de construction

La sélection de la chaîne de construction est possible _(initialement)_ en ligne ou commande.

```bash
cmake -S ${SOURCE_DIR} -B ${BUILD_DIR} -G ${TOOLCHAIN_NAME} ...
```

Les principales chaînes de construction sont les suivantes.

| Nom                              | Description                                              |
|----------------------------------|----------------------------------------------------------|
| `Visual Studio 14 2015`          | Solution `Visual Studio 2015`                            |
| `MinGW Makefiles`                | Fichiers `Makefile` compatibles avec `mingw32-make`      |
| `Unix Makefiles`                 | Fichiers `Makefile` compatibles avec `make`              |
| `Ninja`                          | Fichiers `build.ninja` compatibles avec `ninja`          |
| `Ninja Multi-Config`             | Fichiers `build-<Config>.ninja` compatibles avec `ninja` |
| `Eclipse CDT4 - MinGW Makefiles` | Projet `Eclipse CDT4` compatible avec `mingw32-make`     |
| `Eclipse CDT4 - Ninja`           | Projet `Eclipse CDT4` compatible avec `ninja`            |
| `Eclipse CDT4 - Unix Makefiles`  | Projet `Eclipse CDT4` compatible avec `make`             |

### Sélection de la configuration

La sélection de la configuration _(pour les chaînes de construction uniques)_ est possible _(initialement)_ en ligne ou commande.

```bash
cmake -S ${SOURCE_DIR} -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ...
```

Les principales configurations sont les suivantes.

| Nom              | Description                                |
|------------------|--------------------------------------------|
| `DEBUG`          | Configuration orientée débogage            |
| `RELEASE`        | Configuration orientée livrable            |
| `MINSIZEREL`     | Configuration orientée système embarqué    |
| `RELWITHDEBINFO` | Configuration hybride livrable et débogage |

### Sélection du mode d'import d'un sous-projet

La sélection du mode d'import d'un sous-projet est possible _(ultérieurement)_ en ligne ou commande ou via l'application `cmake-gui`.

```bash
cmake -S ${SOURCE_DIR} -B ${BUILD_DIR} -D${MySubProject_NAME}_MODE=${MySubProject_MODE} ...
```

### Actualisation automatique des dépendances

L'absence d'actualisation automatique des dépendances est possible en ligne de commande ou en positionnant la variable d'environnement `DD_NOUPDATE`.

```bash
export DD_NOUPDATE=1
cmake -S ${SOURCE_DIR} -B ${BUILD_DIR} ...
```

## 2.14. Construction du projet

Il convient de construire le projet en ligne de commande, via l'application `cmake-gui` ou l'environnement de développement _(`Visual Studio 2015` ou `Eclipse CDT4`)_ sélectionné.

```bash
cmake --build ${BUILD_DIR} --config ${BUILD_TYPE} --target all --parallel
```

### Exécution des tests

L'exécution des tests est possible en ligne de commande ou via l'environnement de développement sélectionné.

```bash
cmake --build ${BUILD_DIR} --config ${BUILD_TYPE} --target test [--parallel]
# Note: Activer le parallélisme seulement si l'exécution des tests est compatible (absence d'accès simultané aux fichiers)
```

### Installation du projet

L'installation du projet est possible en ligne de commande ou via l'environnement de développement sélectionné.

```bash
cmake --build ${BUILD_DIR} --config ${BUILD_TYPE} --target install
```

### Paquetage du projet

Le paquetage du projet est possible en ligne de commande ou via l'environnement de développement sélectionné.

```bash
cmake --build ${BUILD_DIR} --config ${BUILD_TYPE} --target package
```

# 3. Informations complémentaires

## 3.1. Conseils

* Comprendre et assimiler les bases et subtilités du langage [CMake](https://cmake.org/cmake/help/latest/).
* Comprendre et assimiler l'utilisation des modules du projet [`CMake.Common.Modules`](MANUAL.md).
* Vérifier qu'une [variable native](https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html) n'assure pas déjà le besoin souhaité.
* Restreindre la portée des variables simples dans leur périmètre local.
* Augmenter la portée des variables simples dans leur périmètre parent via la propriété `PARENT_SCOPE`.
* Privilégier les variables cache internes si la définition du projet est susceptible de pouvoir en modifier les valeurs.
* Privilégier les variables cache si l'utilisateur est susceptible de pouvoir en modifier les valeurs.
* Surcharger les variables cache dans les modules communs `Common*.cmake` ou le fichier principal `CMakeLists.txt` selon le périmètre souhaité.
* Factoriser au maximum les définitions dans des modules, des variables et des fonctions.
* Privilégier l'utilisation de la fonction `target_include_directories` plutôt que `ìnclude_directories` afin de restreindre le périmètre à la cible.
* Privilégier l'utilisation de la fonction `target_link_libraries` plutôt que `link_directories` afin de restreindre le périmètre à la cible.
* Privilégier l'utilisation de la fonction `add_compile_definitions` plutôt que `add_definitions` afin de restreindre le périmètre à la construction.
* Ne pas hésiter à se documenter et poser des questions aux personnes compétentes.

## 3.2. Contacts

Les responsables du projet `CMake.Common.Modules` sont les suivants.

* [Alexis FOERSTER](mailto:alexis.foerster@csgroup.eu), auteur du projet `CMake.Common.Modules`.
* [Stéphane RIONDET](mailto:stephane.riondet@csgroup.eu), architecte du projet _DLPX-PI_.

## 3.3. Documentation CMake

La documentation CMake est disponible [ici](https://cmake.org/cmake/help/latest/).
