# Getting Started

## Prerequisites

Ensure you have the following installed:

- CMake (version 3.27 or higher).
- A supported C++ compiler.

## Installation

There are several ways to integrate CSToolkit into your project. Choose the method that best suits your needs.

### Method 1: Git Submodule (Recommended)

Add CSToolkit as a Git submodule to your project:

```bash
git submodule add https://<repo>.git cstoolkit
git submodule update --init
```

Then in your `CMakeLists.txt`:

```cmake
include(<path-to-cstoolkit>/CSToolkit.cmake)
```

### Method 2: Direct Download

1. Download the latest release from the repository
2. Extract it to your project's directory (e.g., `cstoolkit`)
3. Add to your `CMakeLists.txt`:

```cmake
include(<path-to-cstoolkit>/CSToolkit.cmake)
```