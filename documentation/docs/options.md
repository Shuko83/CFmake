# Options

!!! warning

    Options and variables should be set before inclusion of CSToolkit

## Configuration Options

### `CSTOOLKIT_AUTO_FIND_PACKAGE`
- **Default**: `ON`
- **Description**: Automatically calls `find_package` on unknown libraries for all targets.

### `CSTOOLKIT_CHECK_DEPENDENCIES`
- **Default**: `ON`
- **Description**: Raises a warning if a target dependency is not defined.

### `CSTOOLKIT_DISABLE_COMMON_CPPRULES`
- **Default**: `OFF`
- **Description**: Disables the inclusion of common C++ rules defined in [CppRules.cmake](cpprules.md).

### `CSTOOLKIT_USE_GIT_TAG_VERSION`
- **Default**: `OFF`
- **Description**: Sets the project version from a Git tag or branch name, if available.

### `CSTOOLKIT_CPACK_RULES`
- **Default**: `OFF`
- **Description**: Adds CPack configuration to the project.

### `CSTOOLKIT_AUTO_DEPLOY_QT`
- **Default**: `ON`
- **Description**: Automatically adds Qt deployment rules for all Qt-dependent executables.

### `CSTOOLKIT_PREFIX_OUTPUT_NAME`
- **Default**: `OFF`
- **Description**: If ON, all targets ouput file are prefixed by project name.
