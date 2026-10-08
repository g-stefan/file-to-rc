# Output format

```
file-to-rc --name=EVEN --file-in=even.bin --file-out=Data.Source.rc
```

with `even.bin` = the 4 bytes `ABCD` (`0x41 0x42 0x43 0x44`) writes:

```rc
EVEN RCDATA {
	0x4241,0x4443
}
```

A bigger file, 64 bytes:

```rc
DATA RCDATA {
	0x9E16,0xCBAA,0xEF60,0x259F,0x41FE,0x9A36,0xEF96,0xC9FA,0xBFCC,0x39CE,0x93A0,0xC373,0x189E,0x364B,0x24DC,0x1666,
	0x631B,0x90C8,0x0678,0x6659,0x0A87,0xC86D,0x92A3,0xD595,0xA273,0xB17B,0x3C36,0x33D8,0xFE2D,0x663F,0x007D,0xD210
}
```

- `<name> RCDATA {` on its own line, then the words, tab indented, 16 per
  line (32 input bytes), separated by `,` (no spaces), each line but the
  last ending with `,`; then `}` on its own line. `\n` line ends.
- Every **two input bytes** become one 16 bit word `0xHHHH` (upper case
  hex, four digits). The first byte is the **low** byte of the word:
  `A B` → `0x4241`.
- `rc.exe` stores a number in `RCDATA` as a 16 bit little endian `WORD`,
  so `0x4241` is stored as `41 42` again: the compiled resource holds the
  exact bytes of the file (checked with `rc.exe` 10.0.26100, byte by byte,
  on inputs from 0 bytes to a 2 MB random file).
- The input is read with the machine's byte order; every supported target
  (x86, x64, ARM64) is little endian, which is what the round trip needs.

## Odd sizes

The words are read two bytes at a time. When one byte is left, the missing
high byte is `0x0A`:

```
file-to-rc --name=ODD --file-in=odd.bin --file-out=Data.Source.rc     # odd.bin = "ABC"
```

```rc
ODD RCDATA {
	0x4241,0x0A43
}
```

The resource is then **one byte longer** than the file, and its last byte
is `0x0A` (a line feed): `SizeofResource` returns 4 for the 3 byte file.
For text that is a harmless trailing newline; for binary data whose
format checks the exact length, store the real size somewhere else or pad
the input to an even size yourself.

## Empty input

```rc
EMPTY RCDATA {
}
```

`rc.exe` accepts it and creates a resource of size 0.

## Line length

Lines are at most 113 characters (a tab and 16 words), whatever the input
size. This matters: `rc.exe` reads long lines in pieces of about 8 KB and
a `0xHHHH` token cut at the edge of a piece is either rejected
(`error RC2021: expected exponent value`) or read as two numbers, which
silently adds bytes to the resource. Versions of `file-to-rc` before the
16 word lines wrote each resource on one line and were only correct for
inputs up to about 2 KB; regenerate `.rc` files made by them.

The `.rc` is about 3.5 times the size of the input, and `rc.exe` parses
every word: for big files (megabytes) referencing the file directly,
`logo RCDATA "logo.png"`, is faster to compile, if the `.rc` does not
need to be self-contained.

## Resource names

`--name` is written as is, in front of `RCDATA`. `rc.exe` then decides
what it is:

| `--name` | Resource name | Find it with |
|----------|---------------|--------------|
| `logoPng` | the string `LOGOPNG` (string names are stored upper case) | `FindResourceA(module, "logoPng", ...)` (lookup ignores case) |
| `101` (only digits) | the numeric id 101 | `MAKEINTRESOURCEA(101)` |
| `IDR_LOGO`, with `#define IDR_LOGO 101` seen before the line | the numeric id 101 (the preprocessor replaces it) | `MAKEINTRESOURCEA(IDR_LOGO)` |
| `logo.png`, `a,b`, `my-logo` | accepted as strings (`LOGO.PNG`, ...) | the same string |
| `"my logo"`, anything with a space | `rc` error | — |

Keep to identifiers or numbers; then the name works the same in the `.rc`
and in C++.

## Reading the resource at run time

The type is `RCDATA` (`RT_RCDATA`, `MAKEINTRESOURCE(10)`). The data stays
in the mapped image: no copy, no free, valid while the module is loaded.

```cpp
#include <windows.h>

bool getResource(HMODULE module, const char *name, const void *&data, size_t &size) {
	HRSRC resource = FindResourceA(module, name, MAKEINTRESOURCEA(10)); // RT_RCDATA
	if (resource == nullptr) {
		return false;
	};
	HGLOBAL handle = LoadResource(module, resource);
	if (handle == nullptr) {
		return false;
	};
	data = LockResource(handle);
	size = SizeofResource(module, resource);
	return data != nullptr;
};

const void *data;
size_t size;
if (getResource(nullptr, "DEFAULTCONFIG", data, size)) {
	// remember: size is rounded up to even (odd sized inputs end with 0x0A)
};
```

- `module = nullptr` is the `.exe`. For a resource compiled into a DLL,
  pass the DLL's `HMODULE` (`GetModuleHandleA("my.dll")`, or the
  `hinstDLL` of `DllMain`).
- The data is **not** `0x00` terminated. To use it as a C string, copy it
  (`String text; text.set(static_cast<const char *>(data), size);` with
  `xyo-encoding`'s `String`).
- For a numeric id use `MAKEINTRESOURCEA(101)` as `name`.

## Appending

With `--append` (library: `append = true`) the next resource is written
right after the previous one:

```rc
EVEN RCDATA {
	0x4241,0x4443
}
ODD RCDATA {
	0x4241,0x0A43
}
```

Each name must be unique in the final executable.

## Pitfalls

| Situation | What happens | Do this |
|-----------|--------------|---------|
| a `.rc` generated by an old `file-to-rc` (one line per resource) from more than about 2 KB | `rc.exe` fails with `RC2021`, or silently adds bytes | regenerate it with the current version |
| input of several MB | a `.rc` 3.5 times bigger, slow `rc` | reference the file (`NAME RCDATA "file"`) if the `.rc` need not be self-contained |
| input of odd size | resource is one byte longer, ends with `0x0A` | expect it, or pad the input to an even size |
| the generated `.rc` is in the source folder and named `*.rc` | fabricare compiles it on its own **and** through the `#include`: duplicate resource, `CVT1100` at link | name it `*.Source.rc` |
| `--append` run again on the next build | the output grows, duplicate resources, `CVT1100` | start every sequence with one call without `--append` |
| two resources with the same name in one module | `CVTRES : fatal error CVT1100: duplicate resource` | unique `--name` per resource |
| a name with a space | `rc` error | identifiers or numbers |
| the `#define` for a numeric id comes after the generated line | the name stays a string, `MAKEINTRESOURCEA(id)` finds nothing | include the `.rh` before the generated file |
| treating the data as a C string | reads past the end, no terminator | copy with the size from `SizeofResource` |
| Linux / Emscripten | resources do not exist there | use [file-to-cs](https://github.com/g-stefan/file-to-cs) to embed data portably |
