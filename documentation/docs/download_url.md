# Download URL

## Overview
The `cstoolkit_download_url` function downloads files and optionally extracts them.

### Syntax
```cmake
cstoolkit_download_url(<url> <output> [NO_EXTRACT])
```

### Parameters
- **`<url>`**: The URL of the file to download.
- **`<output>`**: The variable to store the path of the downloaded file or extracted folder.
- **`NO_EXTRACT`**: Prevents automatic extraction of the file.