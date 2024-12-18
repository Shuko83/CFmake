# Welcome to the CSToolkit CMake Utility Documentation

CSToolkit is a powerful CMake utility module designed for BLSOI CSGROUP that simplifies and streamlines the CMake build process. It provides a collection of high-level functions and utilities that make it easier to manage complex C++ projects.

## Key Features

- **Simplified Target Management**: Create and configure targets with a single, intuitive function call
- **Artifactory Integration**: Seamless integration with Artifactory for dependency management
- **Qt Support**: Built-in support for Qt projects including UI, resources, and translations
- **Automatic File Discovery**: Smart file discovery and organization based on file types
- **Cross-Platform**: Works across different platforms with special support for Windows
- **Plugin System**: Built-in support for plugin-based architectures

## Quick Start

```cmake
# Include CSToolkit
include(<path-to-cstoolkit>/CSToolkit.cmake)

# Create an executable with automatic file discovery
cstoolkit_add_target(MyApp EXECUTABLE
    RECURSIVE
    PUBLIC_HEADERS_DIRS include
    SOURCES_DIRS src
    PUBLIC_LINK_LIBRARIES
        Qt5::Widgets
)
```

## Requirements

- CMake 3.27 or higher
- Cpp11 compatible compiler
- Git (optional, for version management)
- Qt5 (optional, for Qt features)

## Quick Links
- [Getting Started](getting_started.md)
- [Configuration Options](variables.md)
- [Core Functions](add_target.md)

## License

This project is licensed under the terms specified by CSGROUP.