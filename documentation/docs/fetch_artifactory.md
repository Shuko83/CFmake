# Fetching Packages

## Overview

The `cstoolkit_fetch_artifactory` macro simplifies the process of fetching and managing external dependencies from an Artifactory server. It supports fetching specific versions of packages, including legacy packages, and integrates them into the current project using CMake's `FetchContent` module.

---

## Syntax

```cmake
cstoolkit_fetch_artifactory(<FETCH_NAME>
                            [URL <url>]
                            [VERSION <version>]
                            [FOLDER <folder>]
                            [ALIAS <alias>]
                            [QT]
                            [LEGACY])
```

### Parameters

- **`FETCH_NAME`** *(required)*:
  The name of the package to fetch. Used to construct the default fetch URL if `URL` is not provided.

- **`URL`** *(optional)*:
  A direct URL to the package. If not specified, a URL is constructed using `VERSION`, `FOLDER`, and other options.

- **`VERSION`** *(optional)*:
  The version of the package to fetch. Used to construct the default fetch URL.

- **`FOLDER`** *(optional)*:
  The folder path within the Artifactory repository where the package resides. Defaults to `"thirdParty"`.

- **`ALIAS`** *(optional)*:
  An alternative name for the package. Useful for referencing the package in `find_package`.

- **`QT`** *(optional)*:
  Indicates that the package is a Qt-related dependency. Appends the appropriate Qt version and mkspecs to the constructed URL.

- **`LEGACY`** *(optional)*:
  Fetches the package in legacy mode, ensuring compatibility with older configurations. The resulting URL uses a `.zip` file format instead of `.7z`.

---

## Behavior

1. **URL Construction**:
   If `URL` is not provided, a default URL is constructed based on:
   - Artifactory base URL (`CSTOOLKIT_ARTIFACTORY_URL`).
   - `FOLDER`, `FETCH_NAME`, `VERSION`.
   - Qt-specific options (`QT` and `LEGACY`).

2. **Fetch Mechanism**:
   - Uses `FetchContent_Declare` and `FetchContent_MakeAvailable` to download and integrate the package into the project.
   - Ensures packages are downloaded only once, even if referenced multiple times.

3. **Legacy Mode**:
   - Generates a compatible `Config.cmake` file for older packages.
   - Uses `.zip` format for the package archive.

4. **Integration**:
   - Sets up `find_package` hints for the fetched package.
   - Supports aliasing for alternative package names.

5. **Caching**:
   - Caches the fetched URL to avoid redundant downloads.

6. **Performance Metrics**:
   - Logs the time taken to fetch the package.

---

## Example Usages

### Fetching a Specific Version

```cmake
cstoolkit_fetch_artifactory(MyLibrary VERSION 1.2.3)
```

This constructs a default URL and fetches version `1.2.3` of `MyLibrary`.

---

### Fetching with a Custom URL

```cmake
cstoolkit_fetch_artifactory(MyLibrary URL https://example.com/artifacts/mylibrary.zip)
```

This directly downloads the package from the provided URL.

---

### Fetching a Library in Legacy Mode

```cmake
cstoolkit_fetch_artifactory(QtLibrary URL https://example.com/artifacts/mylibrary.zip LEGACY)
```

---

## Notes

- At least one of `URL` or `VERSION` must be specified.
- If both `QT` and `LEGACY` are used, the constructed URL appends the appropriate mkspecs and file extension.
- Packages fetched in `LEGACY` mode have a custom-generated `Config.cmake` file to ensure compatibility with modern projects.

---

## Error Handling

- **Missing Parameters**:
  If neither `URL` nor `VERSION` is provided, the macro raises an error:
  ```
  CSToolkit: cstoolkit_fetch_artifactory: VERSION or URL parameter must be defined.
  ```

- **Alias Usage**:
  If `ALIAS` is provided, `find_package` requires the alias name.

---

## Performance

The macro logs the time taken to fetch a package in the following format:
```
CSToolkit Fetch MyLibrary - done (1.2s)
```
