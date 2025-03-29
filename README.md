# cmd_clip
This is a set of two simple Windows utilities for working with files on the clipboard:

- **`fcopy`**: Copies files to the clipboard, similar to if you selected them in Windows Explorer and pressed Ctrl+C.
- **`fpaste`**: Pastes files from the clipboard to the current directory.

## Building

Tested with WinLibs using G++ and Mingw32-make.

To build:

```sh
mingw32-make
```

This will generate the `fcopy` and `fpaste` executables in the `bin` directory.

## Usage

### `fcopy`

Copy one or more files to the clipboard.

```sh
fcopy <file1> [file2 ...]
```

### `fpaste`

Paste files from the clipboard into the current directory.

```sh
fpaste
```

## Clean

To remove build artifacts:

```sh
mingw32-make clean
```
