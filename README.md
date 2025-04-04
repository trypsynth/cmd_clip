# cmd_clip
This is a set of Windows utilities for working with files on the clipboard in a format that Windows Explorer and similar programs can understand:
* `fcopy`: Copies files to the clipboard, similar to selecting them in Windows Explorer and pressing Ctrl+C.
* `fcut: Moves files to the clipboard, similar to Ctrl+X in Windows Explorer.
* `fpaste`: Pastes files from the clipboard into the current directory.
* `fclear`: Clears the clipboard of any copied or cut files.
* `flist`: Lists files currently stored on the clipboard.

## Building
To build with CMake, follow these steps:
```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

## Usage

### `fcopy` – Copy files to the clipboard
```sh
fcopy <file1> [file2 ...]
```

### `fcut` – Cut (move) files to the clipboard
```sh
fcut <file1> [file2 ...]
```

### `fpaste` – Paste files from the clipboard
```sh
fpaste
```

### `fclear` – Clear the clipboard
```sh
fclear
```

### `flist` – List files currently on the clipboard
```sh
flist [-i]
```

-i will display much more detailed information about the files on your clipboard.
