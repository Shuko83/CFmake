## Example Projects

The CSToolkit comes with several example projects demonstrating different use cases:

### Basic Project Structure
```cmake
# Root CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(MyProject)

# Set options before including CSToolkit
set(CSTOOLKIT_DEFAULT_RESOURCES_DIRS "resources")
set(CSTOOLKIT_CPACK_RULES ON)

# Include CSToolkit
include(CSToolkit.cmake)

# Add your subprojects
add_subdirectory(MyExe)
add_subdirectory(MyLib)
# ... more subprojects
```

### Basic Executable (MyExe)
```cmake
# Simple executable with dependencies
cstoolkit_add_target(MyExe EXECUTABLE RECURSIVE
    PRIVATE_LINK_LIBRARIES MyStaticLib
    PRIVATE_LINK_LIBRARIES MyLib
)

# Deploy additional files with the executable
cstoolkit_deploy_with_target(MyExe
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/deployed/"
    DESTINATION "config"
)
```

### Shared Library (MyLib)
```cmake
# Simple shared library with external dependencies
cstoolkit_add_target(MyLib SHARED RECURSIVE RECURSIVE_INTERFACE_INCLUDE
    PUBLIC_LINK_LIBRARIES tinyxml2
)
```

### Qt Integration (MyQtLib)
```cmake
# Qt library example
cstoolkit_add_target(MyQtLib SHARED RECURSIVE
    PUBLIC_LINK_LIBRARIES Qt5::Widgets
)
```

### Available Examples
The following examples are provided in the `example` directory:

- **MyExe**: Basic executable with dependencies
- **MyLib**: Shared library with external dependencies
- **MyQtLib**: Library using Qt framework
- **MyQtExe**: Executable using Qt framework
- **MyStaticLib**: Static library example
- **MySharedAndStaticLib**: Example showing both shared and static libraries
- **MyCombinedStaticLib**: Combined static library example
- **MyExeWithPlugins**: Executable with plugin support
- **MyPlugin**: Plugin implementation example

Each example demonstrates different aspects of CSToolkit's capabilities and can be used as a starting point for your own projects.

### Artifactory Integration
CSToolkit provides easy integration with Artifactory for managing dependencies:

```cmake
# Download package from Artifactory
cstoolkit_fetch_artifactory("TinyXML2" VERSION "5.0.1-2")
#cstoolkit_fetch_artifactory("Boost" VERSION "1.78.0")
```