# Variables

The CMake Utility Module supports a variety of configuration variables to customize its behavior.

!!! warning

    Options and variables should be set before inclusion of CSToolkit

## Key Variables

### `CSTOOLKIT_PROJECT_VERSION`
- **Default**: `0.0.0.0`
- **Description**: Sets the project version. Updates `CMAKE_PROJECT_VERSION` and `PROJECT_VERSION`.

### `CSTOOLKIT_INSTALL_TARGETS`
- **Default**: Empty (installs all targets).
- **Description**: A list of specific targets to install.

### `CSTOOLKIT_DEFAULT_PUBLIC_HEADERS_DIRS`
- **Default**: `include`
- **Description**: Default directory for public headers.

### `CSTOOLKIT_DEFAULT_PRIVATE_HEADERS_DIRS`
- **Default**: `src`
- **Description**: Default directory for private headers.

### `CSTOOLKIT_DEFAULT_SOURCES_DIRS`
- **Default**: `src`
- **Description**: Default directory for source files.

### `CSTOOLKIT_DEFAULT_UI_DIRS`
- **Default**: `src`
- **Description**: Default directory for Qt `.ui` files.

### `CSTOOLKIT_DEFAULT_RESOURCES_DIRS`
- **Default**: `src`
- **Description**: Default directory for Qt `.qrc` files.

### `CSTOOLKIT_DEFAULT_TRANSLATION_DIRS`
- **Default**: `src`
- **Description**: Default directory for Qt `.ts` files.

### `CSTOOLKIT_QT_BIG_RESOURCES_THRESHOLD`
- **Default**: `100000000`
- **Description**: "Size threshold in octet to switch to Qt Big Resources instead of classic resources serialization.
