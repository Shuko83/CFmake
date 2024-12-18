# Adding Targets

## Overview
The `cstoolkit_add_target` function simplifies the creation of CMake targets, including executables, shared libraries, static libraries, and interfaces. It provides extensive configuration options to manage sources, headers, dependencies, plugins, and installation paths.

### Syntax
```cmake
cstoolkit_add_target(<name> {EXECUTABLE | SHARED | STATIC | INTERFACE | MODULE | SHARED_AND_STATIC}
                     [RECURSIVE] [RECURSIVE_INCLUDE] [RECURSIVE_INTERFACE_INCLUDE]
                     [NO_INSTALL] [PUBLIC_HEADERS_NO_EXTENSION]
                     [NAMESPACE <namespace>] [ALIAS <alias>] [EXTENSION <ext>]
                     [PLUGINS_DIR <plugins_dir>]
                     [PUBLIC_LINK_LIBRARIES <library>...]
                     [PRIVATE_LINK_LIBRARIES <library>...]
                     [COMBINED_LINK_LIBRARIES <library>...]
                     [PUBLIC_HEADERS_DIRS <dir>...]
                     [PRIVATE_HEADERS_DIRS <dir>...]
                     [SOURCES_DIRS <dir>...]
                     [UI_DIRS <dir>...]
                     [RESOURCES_DIRS <dir>...]
                     [TRANSLATION_DIRS <dir>...]
                     [PUBLIC_HEADERS_FILES <file>...]
                     [PRIVATE_HEADERS_FILES <file>...]
                     [SOURCES_FILES <file>...]
                     [UI_FILES <file>...]
                     [RESOURCES_FILES <file>...]
                     [TRANSLATION_FILES <file>...]
                     [DEPLOY_FILES <file>...]
                     [LINK_OPTIONS <option>...]
                     [COMPILE_DEFINITIONS <definition>...]
                     [COMPILE_OPTIONS <option>...]
                     [RC_ICONS <icon>...]
                     [PLUGINS <plugin>... [DESTINATION <plugins_dir>]...]
)
```

### Key Parameters

#### General Configuration
- **`<name>`**: The target name. This is mandatory.
- **`{EXECUTABLE | SHARED | STATIC | INTERFACE | MODULE | SHARED_AND_STATIC}`**: The target type.
  - `EXECUTABLE`: Creates an executable target.
  - `SHARED`: Creates a shared library target.
  - `STATIC`: Creates a static library target.
  - `INTERFACE`: Creates an interface target.
  - `MODULE`: Creates a module (plugin library not linked directly).
  - `SHARED_AND_STATIC`: Creates both shared and static libraries.

#### Options
- **`RECURSIVE`**: Searches directories recursively for files to include in the target.
- **`RECURSIVE_INCLUDE`**: Adds all subdirectories to private include paths (not recommended).
- **`RECURSIVE_INTERFACE_INCLUDE`**: Adds all subdirectories to interface include paths (not recommended).
- **`NO_INSTALL`**: Disables installation rules for the target.
- **`PUBLIC_HEADERS_NO_EXTENSION`**: Includes files without extensions as public headers.
- **`NAMESPACE`**: Specifies the namespace for the target (defaults to ${PROJECT_NAME}::).
- **`ALIAS`**: (Deprecated) Use NAMESPACE instead.
- **`EXTENSION`**: Specifies a custom file extension for the target.

#### Linking and Dependencies
- **`PUBLIC_LINK_LIBRARIES`**: Libraries that are publicly linked and exposed in the interface.
- **`PRIVATE_LINK_LIBRARIES`**: Libraries that are privately linked and not exposed.
- **`COMBINED_LINK_LIBRARIES`**: Libraries to be combined into static targets.

#### Directories
- **`PUBLIC_HEADERS_DIRS`**: Directories containing public headers.
- **`PRIVATE_HEADERS_DIRS`**: Directories containing private headers.
- **`SOURCES_DIRS`**: Directories containing source files.
- **`UI_DIRS`**: Directories containing Qt UI files.
- **`RESOURCES_DIRS`**: Directories containing Qt resource files.
- **`TRANSLATION_DIRS`**: Directories containing Qt translation files.

#### Files
- **`PUBLIC_HEADERS_FILES`**: Individual public header files.
- **`PRIVATE_HEADERS_FILES`**: Individual private header files.
- **`SOURCES_FILES`**: Individual source files.
- **`UI_FILES`**: Individual Qt UI files.
- **`RESOURCES_FILES`**: Individual Qt resource files.
- **`TRANSLATION_FILES`**: Individual Qt translation files.
- **`DEPLOY_FILES`**: Additional files to deploy with the target.

#### Build Configuration
- **`LINK_OPTIONS`**: Custom linker options.
- **`COMPILE_DEFINITIONS`**: Custom preprocessor definitions.
- **`COMPILE_OPTIONS`**: Custom compiler options.
- **`RC_ICONS`**: Windows resource icons for executables.

#### Plugins
- **`PLUGINS <plugin>... [DESTINATION <plugins_dir>]...`**: Specifies runtime plugins and their deployment destinations.
- **`PLUGINS_DIR`**: (Deprecated) Use PLUGINS with DESTINATION instead.

### Installation Behavior

The function handles installation based on the following rules:

1. **Top-Level Project Targets**:
   - If `CSTOOLKIT_INSTALL_TARGETS` contains only the target name:
     - Single component mode with root installation paths
   - If `CSTOOLKIT_INSTALL_TARGETS` contains multiple targets including this one:
     - Multi-component mode with target-specific subdirectories
   - If target is not in `CSTOOLKIT_INSTALL_TARGETS`:
     - Installed to internals/ subdirectory and excluded from default installation

2. **Non-Top-Level Project Targets**:
   - Installed to externals/ subdirectory
   - Excluded from default installation
   - Organized by namespace

3. **Installation Paths**:
   - Libraries: `lib/<config>/`
   - Executables: `bin/<config>/`
   - Headers: `include/`
   - CMake files: `cmake/`
   - Debug symbols: `symbols/<config>/`

### Examples

#### Creating an Executable with Qt Support
```cmake
cstoolkit_add_target(MyApp EXECUTABLE
    SOURCES_DIRS src
    PUBLIC_HEADERS_DIRS include
    UI_DIRS ui
    RESOURCES_DIRS resources
    PUBLIC_LINK_LIBRARIES
        Qt5::Core
        Qt5::Widgets
    PLUGINS MyPlugin1 DESTINATION plugins/type1
           MyPlugin2 DESTINATION plugins/type2
)
```

#### Creating a Shared Library with Combined Static Dependencies
```cmake
cstoolkit_add_target(MyLib SHARED_AND_STATIC
    SOURCES_DIRS src
    PUBLIC_HEADERS_DIRS include
    PUBLIC_LINK_LIBRARIES CommonLib
    PRIVATE_LINK_LIBRARIES PrivateLib
    COMBINED_LINK_LIBRARIES StaticDep1 StaticDep2
)
```

#### Creating a Plugin Module
```cmake
cstoolkit_add_target(MyPlugin MODULE
    SOURCES_DIRS src
    PRIVATE_HEADERS_DIRS include
    PRIVATE_LINK_LIBRARIES HostLib
)
```