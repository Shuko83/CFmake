CMake Utility Module du département produits BLSOI CSGROUP

Usage:
All you have to do is add a line like this near the top of your root CMakeLists.txt file (but not before your project() call):
include(<path-to-cstoolkit>/CSToolkit.cmake)

Options and variables should be set before inclusion of CSToolkit

Variables :
CSTOOLKIT_PROJECT_VERSION (0.0.0.0) Version of the project, will set CMAKE_PROJECT_VERSION and PROJECT_VERSION

Options:
CSTOOLKIT_AUTO_FIND_PACKAGE (ON) Automatically calls find_package on unknown libraries of all targets
CSTOOLKIT_CHECK_DEPENDENCIES (ON) Raise a warning if a dependency of a target is not defined

Functions:
cstoolkit_add_target(<name> <EXECUTABLE|SHARED|STATIC|INTERFACE>
                     [RECURSIVE] [RECURSIVE_INCLUDE] [NOINSTALL]
                     [ALIAS <alias>] [EXTENSION <ext>] [PLUGINS_DIR <plugins_dir>]
                     [PUBLIC_LINK_LIBRARIES <library>...] [PRIVATE_LINK_LIBRARIES <library>...]
                     [PUBLIC_HEADERS_BASE_DIRS <header_dir>...] [PUBLIC_HEADERS_FILES <file>...]
                     [LINK_OPTIONS <option>...] [COMPILE_DEFINITIONS <definition>...]
                     [COMPILE_OPTIONS <option>...] [PLUGINS <plugin_library>...]
                     [DEPLOY <deploy_file>...]
                     )
Create target <name> of specified type to compile.

RECURSIVE
    Option to recursively search source and header folders to find source and header files. Recommended

RECURSIVE_INCLUDE
    Option to add to include directories paths every header file. Not recommended

NOINSTALL
    Option disable installation of this target.

ALIAS <alias>
    Optionnal alias of the target.

EXTENSION <ext>
    Optionnal extension of target file. Replace the default extension defined by CMake.

PUBLIC_LINK_LIBRARIES <library>...
    These libraries and targets are linked to, and are made part of the link interface.
    See https://cmake.org/cmake/help/latest/command/target_link_libraries.html for more information.

PRIVATE_LINK_LIBRARIES <library>...
    These libraries and targets are linked to, but are not made part of the link interface.
    See https://cmake.org/cmake/help/latest/command/target_link_libraries.html for more information.

COMBINED_LINK_LIBRARIES <library>...
    For static targets only.
    Same as PRIVATE_LINK_LIBRARIES but the libraries will be included in the final librarian archive file.

PUBLIC_HEADERS_BASE_DIRS <header_dir>...
    Specify the directory containing public header files. Default to "include" directory.

PUBLIC_HEADERS_FILES <file>...
    Optionnal. Allows to manually specify headers file.
    Default take all files inside PUBLIC_HEADERS_BASE_DIRS, recursively if RECURSIVE option specified.

LINK_OPTIONS <option>...
    Add options to the link step for executable or shared library.
    See https://cmake.org/cmake/help/latest/command/add_link_options.html for more information.

COMPILE_DEFINITIONS <definition>...
    Add preprocessor definitions to the compilation of source files.

COMPILE_OPTIONS <option>...
    Add options to the compilation of source files.

Plugins :
A Plugin is a shared library needed at execution time but not at compilation time.

PLUGINS <plugin_library>...
    Specify the list of plugins of the target.

PLUGINS_DIR <plugins_dir>
    Optionnal folder name where the plugin will be deployed. By default plugins are deployed next to the target file.


cstoolkit_fetch_artifactory(<name> URL <url>
                            [ALIAS <alias>]
                            )
Download package <name> available at <url>

cstoolkit_fetch_artifactory(<name> VERSION <version>
                            [ALIAS <alias>]
                            [FOLDER <folder>]
                            )
Download package <name> version <version> available on artifactory with normalized format.
