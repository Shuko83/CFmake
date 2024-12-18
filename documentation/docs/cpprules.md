# Cpp Rules

`CppRules.cmake` enforces coding standards and compilation settings across different platforms and compilers. This document outlines the key rules and configurations.

These rules can be disabled using the `CSTOOLKIT_DISABLE_COMMON_CPPRULES` option. See [Options](options.md) for more details.

## Common Configuration

- C++11 standard is enforced and required
- C++ extensions are disabled
- Debug builds use 'd' postfix
- Symbol visibility is set to hidden by default
- Parallel compilation is enabled for Unix Makefiles based on processor count

## Platform-Specific Rules

### Windows
- Enables Unicode character set by default
- Defines standard Windows platform macros (`_WIN32`, `WIN64` for 64-bit)
- Enables mathematical constants in C/C++ headers
- Uses lean Windows headers (excludes Cryptography, DDE, RPC, Shell, and Windows Sockets)

### Linux
- Defines standard Linux platform macros (`__linux__`)
- Custom platform macro `_LINUX_PLATEFORM_`

## Compiler-Specific Rules

### MSVC (Microsoft Visual C++)
- Enables multi-processor compilation (-MP)
- Uses warning level 4 (-W4)
- Disables incremental linking
- Enforces strict string literal handling
- Enables debug info in Release builds
- Configurations: Debug and Release

### GCC
- Enables comprehensive warnings:
  - -Wall: All standard warnings
  - -Wextra: Extra warnings
  - -Wconversion: Type conversion warnings
  - -Wsign-conversion: Sign conversion warnings
  - -Wold-style-cast: C-style cast warnings
- Strips symbols in Release builds

### Clang
- Enables all warnings (-Weverything)
- Disables specific warnings:
  - -Wno-padded: Padding warnings
  - -Wno-c++98-compat: C++98 compatibility warnings
  - -Wno-c++98-compat-pedantic: C++98 pedantic compatibility warnings
- Strips symbols in Release builds
