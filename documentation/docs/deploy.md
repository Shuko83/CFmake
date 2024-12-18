# Deploying with Target

This function automates the deployment of files or directories alongside a target, both for build-time deployment and installation.

### Synopsis

```cmake
cstoolkit_deploy_with_target(<target> FILES <file>... [DESTINATION <dir>])
cstoolkit_deploy_with_target(<target> DIRECTORY <dir>... [DESTINATION <dir>])
```

### Parameters

- `target`: The target to associate the deployment with. Must be an existing target.
- `FILES`: Deploy individual files.
- `DIRECTORY`: Deploy entire directories.
- `DESTINATION`: Optional. Specifies the destination directory relative to the target's output directory.

### Description

The function performs two main tasks:
1. Copies the specified files/directories to the target's output directory during build
2. Sets up installation rules for the deployed files/directories

### Examples

```cmake
# Deploy individual files
cstoolkit_deploy_with_target(myapp FILES
    "config.json"
    "data.xml"
    "$<TARGET_FILE:MyPlugin>"
    DESTINATION config
)

# Deploy entire directories
cstoolkit_deploy_with_target(myapp DIRECTORY
    "assets/textures/"
    "assets/models"
    "$<TARGET_FILE_DIR:MyCombinedStaticLib>"
    DESTINATION resources
)
```