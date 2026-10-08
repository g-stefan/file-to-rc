---
name: file-to-rc
description: >-
  How to use file-to-rc, the XYO tool and C++ library (namespace
  XYO::FileToRC) on top of xyo-system that converts any file into Windows
  resource script source, an inline RCDATA resource (name RCDATA
  {0xHHHH,...}), to compile it into an .exe / .dll with rc.exe and read it
  with FindResource / LoadResource / LockResource / SizeofResource: the
  file-to-rc command line (--name, --file-in, --file-out, --append,
  --touch, @response files, exit codes); the output format (16 bit words,
  low byte first, 16 words per line, odd sizes padded with 0x0A; old
  versions wrote one line per resource, which rc.exe breaks or silently
  corrupts above ~2 KB); resource names (string, numeric id, #define); the function
  fileToRC; embedding the tool through file-to-rc.application.static and
  XYO::FileToRC::Application::Application (fabricare's fileToRC(...)); the
  *.Source.rc + #include + --touch build pattern. Use when writing or
  reviewing code that includes <XYO/FileToRC.hpp> or
  <XYO/FileToRC.Application.hpp>, depends on "file-to-rc" in
  fabricare.json, calls file-to-rc or fileToRC in a build script,
  #includes a generated *.Source.rc, wants to embed a file as a Windows
  RCDATA resource, or when working inside the file-to-rc repository.
---

# file-to-rc

Converts a file into Windows resource script source so the resource
compiler puts it into the program as an `RCDATA` resource. Two faces:

- the **`file-to-rc` command line tool**, run from build scripts (in
  fabricare scripts also as the built-in `fileToRC(...)`);
- the **C++ library** `XYO::FileToRC`: `fileToRC`, plus the tool as a
  class.

Built on `xyo-system` (see the `xyo-system` skill and the layers below it:
their rules apply). The portable sibling, for C/C++ arrays, is
`file-to-cs` (skill `file-to-cs`).

Full documentation: `docs/` in the file-to-rc repository
(`X:\Storage\XYO\Gitea\CPP\file-to-rc\docs` on this machine): README,
getting-started, command-line, **output-format**, library, reference.
The whole implementation is `source/XYO/FileToRC/Library.cpp` and
`source/XYO/FileToRC.Application/Application.cpp`.

## Output

```
file-to-rc --name=EVEN --file-in=even.bin --file-out=Data.Source.rc   # "ABCD"
file-to-rc --name=ODD  --file-in=odd.bin  --file-out=Data.Source.rc --append  # "ABC"
```

```rc
EVEN RCDATA {
	0x4241,0x4443
}
ODD RCDATA {
	0x4241,0x0A43
}
```

Two bytes per `0xHHHH` word, first byte = low byte; `rc.exe` stores words
little endian, so the resource has the file's exact bytes. 16 words (32
bytes) per tab indented line, max 113 characters, `\n` endings. Empty
input → `NAME RCDATA {` / `}` (size 0 resource).

## Pick an approach

| Need | Do |
|------|----|
| A file as a resource, `.rc` self-contained | `file-to-rc --name=n --file-in=f --file-out=X.Source.rc` |
| Several MB and the `.rc` need not be self-contained | `n RCDATA "file.bin"` directly in the `.rc` (rc reads the file; the generated `.rc` would be 3.5× the input) |
| Several resources in one file | first call plain, the rest `--append` |
| Regenerate only if input newer | `--touch=Application.rc` |
| Data on Linux or in portable code | `file-to-cs` instead (resources are Windows only) |

## The standard build pattern

`fabricare/make.prepare.js`:

```js
messageAction("make.prepare");

runInPath("source/XYO/MyTool", function() {
	exitIf(fileToRC("--touch=Application.rc", "--name=defaultConfig", "--file-in=DefaultConfig.json", "--file-out=DefaultConfig.Source.rc"));
});
```

`source/XYO/MyTool/Application.rc`:

```rc
#include <XYO/MyTool/DefaultConfig.Source.rc>
```

Run time (Windows):

```cpp
HRSRC resource = FindResourceA(nullptr, "DEFAULTCONFIG", MAKEINTRESOURCEA(10)); // RT_RCDATA
HGLOBAL handle = LoadResource(nullptr, resource);
const void *data = LockResource(handle);        // not 0x00 terminated, no free needed
DWORD size = SizeofResource(nullptr, resource);  // even: odd inputs gain a 0x0A byte
```

For a DLL pass its `HMODULE` instead of `nullptr`.

## Hard rules

1. **Required options**: `--name`, `--file-in`, `--file-out`. Missing →
   usage printed, exit `1`. Empty value (`--name=`) → `Error: ... is
   empty`, exit `1`.
2. **Silent failures**: if the input can not be read or the output can
   not be written (missing output folder, no access) the tool prints
   **nothing** and exits `1`; `fileToRC` returns `false`. Always
   `exitIf(...)` / check the result. The output folder is never created.
3. **Keep lines short.** `rc.exe` (checked 10.0.26100) reads long lines
   in ~8 KB pieces; a token cut at the edge either fails (`error RC2021:
   expected exponent value`, `fatal error RC1116`) or is read as two
   numbers and the resource **silently gains bytes**. That is why
   `fileToRC` writes 16 words per line; never change it back to one long
   line. Old versions did, and were wrong above ~2.3 KB of input: a
   `*.Source.rc` produced by them must be regenerated.
4. **Odd sizes**: the last word is `0x0AHH`: the resource is one byte
   longer and ends with `0x0A`. Fine for text; for binary formats that
   check their length, store the real size or pad the input.
5. **Name generated files `*.Source.rc`.** fabricare compiles every
   `*.rc` in the source path except names containing `.Source.`; a
   generated `X.rc` that is also `#include`d would be compiled twice →
   duplicate resource. Duplicates are not an `rc` error: the link fails
   with `CVTRES : fatal error CVT1100: duplicate resource`.
6. **`--touch=f`**: if the output exists and the input is **not newer**,
   exit `0` and do nothing (not even touch). Otherwise write, then touch
   `f` only if it exists. Point it at the `.rc` that `#include`s the
   generated file.
7. **`--append` accumulates**: every run adds again. Start each sequence
   with one call without `--append`. Do not mix `--append` with `--touch`
   on a shared output (later inputs are compared to the already written
   file and skipped).
8. **Names**: written as given. String names are stored upper case
   (`logoPng` → `LOGOPNG`; `FindResource` ignores case). Only digits →
   numeric id (`MAKEINTRESOURCEA(101)`). A `#define`d name is replaced by
   its value only if the `#define` comes before the line (include the
   `.rh` first). A space or a quote in the name → `rc` error. Prefer
   identifiers or numbers.
9. **The data is not a C string**: no terminator; copy it with the size
   from `SizeofResource`. It stays valid while the module is loaded.
10. **Generated files** (`*.Source.rc`) are outputs: never edit them, edit
    the input and rebuild.
11. **`@file`** response files are expanded first (split like a command
    line, quotes group words); a missing one → `Error: file not found -
    file`, exit `1`. Unknown options and non `--` arguments are ignored.

## Depend on it

```json
"dependency": [ "file-to-rc" ]                    // DLL
"dependency": [ "file-to-rc.static" ]             // static, defines XYO_FILETORC_LIBRARY
"dependency": [ "file-to-rc.application.static" ] // embed the tool, no main
```

```cpp
#include <XYO/FileToRC.hpp>

if (!XYO::FileToRC::fileToRC("defaultConfig", "DefaultConfig.json", "DefaultConfig.Source.rc", false)) {
	printf("* Error: DefaultConfig.json\n");
	return 1;
};
```

Embedded tool (as fabricare's `fileToRC(...)`): build a plain `char *`
array (`cmdS[0]` = program name, `TDynamicArray` is not contiguous) and
call `XYO::FileToRC::Application::Application application;
application.main(cmdN, cmdS)` — it returns the exit code and prints to
stdout.

Metadata: `XYO::FileToRC::Version::version()` from
`<XYO/FileToRC/Version.hpp>` (not included by `<XYO/FileToRC.hpp>`); the
tool's is `XYO::FileToRC::Application::Version::version()`.

## Working inside this repository

- Four projects in `fabricare.json`: `file-to-rc` (DLL, version key
  `file-to-rc.library`), `file-to-rc.static`,
  `file-to-rc.application.static`, `file-to-rc` (exe).
- `fabricare make`, `fabricare install`. There is no test project; try
  changes with the built `output/bin/file-to-rc` on sample inputs and
  compile the result with `rc.exe` (and link, to catch `CVT1100`).
  Check odd, even, empty and big (> 8 KB of `.rc` text, e.g. a 2 MB
  random file) inputs and compare the resource bytes with the input.
  In a plain shell `cl.exe` is missing: run `fabricare make` after
  `vcvars64.bat`.
- fabricare bootstraps this tool without itself
  (`build/fabricare.compile.json` in the fabricare repository compiles
  `FileToRC.Application.Amalgam.cpp`): keep it self-contained.
- Keep the docs in `docs/` and this skill in step with `Library.hpp` /
  `Application.cpp` when behavior changes (the output samples here and
  in `docs/output-format.md` are real tool output).
- Licensing follows REUSE: `source/`, `docs/` and `README.md` are MIT
  (source files also carry SPDX headers); build scripts, config,
  `version.json` and `.claude/` are Unlicense. Every new top level file or
  folder needs a `Files:` entry in `.reuse/dep5` (check with `reuse lint`).
