# CMake Utility Module du département produits BLSOI CSGROUP

## Usage:
All you have to do is add a line like this near the top of your root CMakeLists.txt file (but not before your project() call):  
`include(<path-to-cstoolkit>/CSToolkit.cmake)`

Options and variables should be set before inclusion of CSToolkit

## Variables :
`CSTOOLKIT_PROJECT_VERSION` *0.0.0.0*  
Version of the project, will set `CMAKE_PROJECT_VERSION` and `PROJECT_VERSION`

`CSTOOLKIT_INSTALL_TARGETS` *\<empty\>*  
List of targets to install, if empty install all targets

`CSTOOLKIT_DEFAULT_PUBLIC_HEADERS_DIRS` *include*  
Default folder for public headers of a target created with cstoolkit_add_target

`CSTOOLKIT_DEFAULT_PRIVATE_HEADERS_DIRS` *src*  
Default folder for private headers of a target created with cstoolkit_add_target

`CSTOOLKIT_DEFAULT_SOURCES_DIRS` *src*  
Default folder for source files of a target created with cstoolkit_add_target

`CSTOOLKIT_DEFAULT_UI_DIRS` *src*  
Default folder for Qt ui files of a target created with cstoolkit_add_target

`CSTOOLKIT_DEFAULT_RESOURCES_DIRS` *src*  
Default folder for Qt qrc files of a target created with cstoolkit_add_target

`CSTOOLKIT_DEFAULT_TRANSLATION_DIRS` *src*  
Default folder for Qt ts files of a target created with cstoolkit_add_target

## Options:
`CSTOOLKIT_AUTO_FIND_PACKAGE` *ON*  
Automatically calls find_package on unknown libraries of all targets

`CSTOOLKIT_CHECK_DEPENDENCIES` *ON*  
Raise a warning if a dependency of a target is not defined

`CSTOOLKIT_DISABLE_COMMON_CPPRULES` *OFF*  
Does not include common DivST Cpp Rules

`CSTOOLKIT_USE_GIT_TAG_VERSION` *OFF*  
Set project version from git tag or branch name if possible

`CSTOOLKIT_CPACK_RULES` *OFF*  
Add CPack configuration to the project

`CSTOOLKIT_AUTO_DEPLOY_QT` *ON*  
Add qt deploy rules to all Qt dependent executables


## Functions:
___
```perl
cstoolkit_add_target(<name> {EXECUTABLE | SHARED | STATIC | INTERFACE}
                     [RECURSIVE] [RECURSIVE_INCLUDE] [RECURSIVE_INTERFACE_INCLUDE]
					 [NOINSTALL] [PUBLIC_HEADERS_NO_EXTENSION]
                     [NAMESPACE <namespace>] [ALIAS <alias>] [EXTENSION <ext>]
                     [PUBLIC_LINK_LIBRARIES <library>...] [PRIVATE_LINK_LIBRARIES <library>...]
					 [COMBINED_LINK_LIBRARIES <library>...]
					 [PLUGINS_DIR <plugins_dir>] [PLUGINS <library>...] 
                     [PUBLIC_HEADERS_DIRS <dir>...] [PRIVATE_HEADERS_DIRS <dir>...]
					 [SOURCES_DIRS <dir>...] [UI_DIRS <dir>...] [RESOURCES_DIRS <dir>...]
					 [TRANSLATION_DIRS <dir>...]
					 [PUBLIC_HEADERS_FILES <file>...] [PRIVATE_HEADERS_FILES <file>...]
					 [SOURCES_FILES <file>...] [UI_FILES <file>...] [RESOURCES_FILES <file>...]
					 [TRANSLATION_FILES <file>...]
					 [DEPLOY_FILES <file>...]
                     [LINK_OPTIONS <option>...] [COMPILE_DEFINITIONS <definition>...]
					 [COMPILE_OPTIONS <option>...]
                     [RC_ICONS <icon>...]
                     )
```
Create target `<name>` of specified type to compile.

`RECURSIVE` **Recommended**  
Option to recursively search defined dirs for files.  
* PUBLIC_HEADERS_DIRS
  * .h .hpp (All files with option `PUBLIC_HEADERS_NO_EXTENSION`)
* PRIVATE_HEADERS_DIRS
  * .h .hpp
* SOURCES_DIRS
  * .c .cpp
* UI_DIRS
  * .ui
* RESOURCES_DIRS
  * .qrc
* TRANSLATION_DIRS
  * .ts

`RECURSIVE_INCLUDE` **Not recommended**  
This option allows to add all include subdirectories to private include directories

`RECURSIVE_INTERFACE_INCLUDE` **Not recommended**  
This option allows to add all include subdirectories to interface include directories

`NO_INSTALL`  
This option disable install rules of this target.

`NAMESPACE <namespace>`  
Optionnal namespace of the target.  
The created target will be accesible in the form NAMESPACE::NAME  
Default to `PROJECT_NAME`.

`ALIAS <alias>`  
Optionnal alias for the target.

`EXTENSION <ext>`  
Optionnal extension of target file. Replaces the default file extension defined by CMake.

`PUBLIC_LINK_LIBRARIES <library>...`  
These libraries and targets are linked to, and are made part of the link interface.  
See https://cmake.org/cmake/help/latest/command/target_link_libraries.html for more information.

`PRIVATE_LINK_LIBRARIES <library>...`  
These libraries and targets are linked to, but are not made part of the link interface.  
See https://cmake.org/cmake/help/latest/command/target_link_libraries.html for more information.

`COMBINED_LINK_LIBRARIES <library>...`  
For static targets only.  
Same as `PRIVATE_LINK_LIBRARIES` but the libraries will be included in the final librarian archive file.

`PUBLIC_HEADERS_DIRS <dir>...`  
Allows to manually specify the directories containing public header files. Default to `CSTOOLKIT_DEFAULT_PUBLIC_HEADERS_DIRS`.

`PRIVATE_HEADERS_DIRS <dir>...`  
Allows to manually specify the directories containing private header files. Default to `CSTOOLKIT_DEFAULT_PRIVATE_HEADERS_DIRS`.

`SOURCES_DIRS <dir>...`  
Allows to manually specify the directories containing source files. Default to `CSTOOLKIT_DEFAULT_SOURCES_DIRS`.

`UI_DIRS <dir>...`  
Allows to manually specify the directories containing Qt ui files. Default to `CSTOOLKIT_DEFAULT_UI_DIRS`.

`RESOURCES_DIRS <dir>...`  
Allows to manually specify the directories containing Qt resources files. Default to `CSTOOLKIT_DEFAULT_RESOURCES_DIRS`.

`TRANSLATION_DIRS <dir>...`  
Allows to manually specify the directories containing Qt translation files. Default to `CSTOOLKIT_DEFAULT_TRANSLATION_DIRS`.

`PUBLIC_HEADERS_FILES <file>...`  
Allows to manually specify public headers file. Override files found by `RECURSIVE` option.  

`PRIVATE_HEADERS_FILES <file>...`  
Allows to manually specify private headers file. Override files found by `RECURSIVE` option.  

`SOURCES_FILES <file>...`  
Allows to manually specify source file. Override files found by `RECURSIVE` option.  

`UI_FILES <file>...`  
Allows to manually specify Qt ui file. Override files found by `RECURSIVE` option.  

`RESOURCES_FILES <file>...`  
Allows to manually specify Qt resources file. Override files found by `RECURSIVE` option.  

`TRANSLATION_FILES <file>...`  
Allows to manually specify Qt translation file. Override files found by `RECURSIVE` option.  

`LINK_OPTIONS <option>...`  
Add options to the link step for executable or shared library.  
See https://cmake.org/cmake/help/latest/command/add_link_options.html for more information.

`COMPILE_DEFINITIONS <definition>...`  
Add preprocessor definitions to the compilation of source files.
See https://cmake.org/cmake/help/latest/command/add_compile_definitions.html for more information.

`COMPILE_OPTIONS <option>...`  
Add options to the compilation of source files.
See https://cmake.org/cmake/help/latest/command/add_compile_options.html for more information.

`RC_ICONS <icon>...`  
Allows to specify icon files for windows executable or shared library.

`DEPLOY_FILES <file>...`  
Allows to specify legacy LDTools deploy files to execute post-build.

**Plugins :**  
A Plugin is a shared library needed at execution time but not at compilation time.

`PLUGINS <plugin_library>...`  
Specify the list of plugins of the target.

`PLUGINS_DIR <plugins_dir>`  
Optionnal folder name where the plugins will be deployed. By default plugins are deployed next to the target file.

___
```perl
cstoolkit_fetch_artifactory(<name> URL <url>
                            [ALIAS <alias>]
                            )
```
Download package `<name>` available at `<url>`.  
Automatically call find_package.

___
```perl
cstoolkit_fetch_artifactory(<name> VERSION <version>
                            [ALIAS <alias>]
                            [FOLDER <folder>]
                            )
```
Download package `<name>` version `<version>` available on artifactory with normalized format.  
`FOLDER` allows to specify path to package (Example: "release/Libraries"). Default to "thirdParty".

___
```perl
cstoolkit_download_url(<url> <output>
                       [NO_EXTRACT]
                       )
```
Download file at `<url>` and store path to download file into `<output>` variable.  
If the downloaded file is extractible and option `NO_EXTRACT` isn't used, `<output>` variable cointains the path to the extracted folder.

