# Getting started

## 1. Build and install

The library and the tool are built with
[fabricare](https://github.com/g-stefan/fabricare). `xyo-platform`,
`xyo-managed-memory`, `xyo-data-structures`, `xyo-multithreading`,
`xyo-encoding` and `xyo-system` must be installed to the SDK first. From
the repository root:

```bash
fabricare make       # build into output/
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

After `install`, `file-to-rc` is in `~/.fabricare/<platform>/bin`, which
is on the `PATH` of a fabricare build, so the scripts of other projects can
call it.

Four projects are produced (`fabricare.json`):

| Project | Kind | Use it when |
|---------|------|-------------|
| `file-to-rc` | DLL / shared library (`dll-or-lib`), the library | default, shared between several programs |
| `file-to-rc.static` | static library, static CRT | self-contained executables |
| `file-to-rc.application.static` | static library with the command line tool, no `main` | embed `file-to-rc` into another tool (fabricare does) |
| `file-to-rc` | executable | the `file-to-rc` command |

The DLL and the executable share the name `file-to-rc`, so the library
keeps its version under the key `file-to-rc.library` in `version.json`
(`"versionName"`); `file-to-rc.static` reuses it and
`file-to-rc.application.static` reuses the executable's (`"linkVersion"`).

The tool and the library build on Windows and Linux; the output is only
useful to a Windows resource compiler (`rc.exe`).

## 2. Use the tool in a build

The usual case: a Windows program keeps a small data file in its source
folder and compiles it in as a resource. In the project's
`fabricare/make.prepare.js`:

```js
messageAction("make.prepare");

runInPath("source/XYO/MyTool", function() {
	exitIf(fileToRC("--touch=Application.rc", "--name=defaultConfig", "--file-in=DefaultConfig.json", "--file-out=DefaultConfig.Source.rc"));
});
```

(`fileToRC(...)` is the tool built into fabricare, one argument per
parameter; `Shell.system("file-to-rc ...")` works too.)

In `source/XYO/MyTool/Application.rc`:

```rc
#include <XYO/MyTool/DefaultConfig.Source.rc>
```

Name the generated file `*.Source.rc`: fabricare compiles every `*.rc` of
the source folder **except** the ones whose name contains `.Source.`, so
the generated file is compiled once, through the `#include`, and not a
second time on its own (that would be a duplicate resource). Do not edit
it; list it in `.gitignore` or commit it. `--touch=Application.rc`
regenerates it only when the input is newer and then touches
`Application.rc`, so the incremental build recompiles the resources. See
[Command line](command-line.md).

At run time:

```cpp
#include <windows.h>

HRSRC resource = FindResourceA(nullptr, "DEFAULTCONFIG", MAKEINTRESOURCEA(10)); // RT_RCDATA
HGLOBAL handle = LoadResource(nullptr, resource);
const char *data = static_cast<const char *>(LockResource(handle));
DWORD size = SizeofResource(nullptr, resource);
```

More in [Output format](output-format.md#reading-the-resource-at-run-time).

## 3. Depend on the library from another fabricare project

In the consumer's `fabricare.json`:

```json
{
	"name": "my-packer",
	"make": "exe",
	"sourcePath": "XYO/MyPacker",
	"dependency": [
		"file-to-rc"
	]
}
```

For the static variant use `"file-to-rc.static"`; it exports
`XYO_FILETORC_LIBRARY` to the consumer (`dependencyDefines`), which turns
`XYO_FILETORC_EXPORT` into nothing. To embed the command line tool use
`"file-to-rc.application.static"` (exports
`XYO_FILETORC_APPLICATION_LIBRARY`, which leaves out `main`). `xyo-system`
and the layers below come in as transitive dependencies.

## 4. Include

```cpp
#include <XYO/FileToRC.hpp>               // the library
#include <XYO/FileToRC.Application.hpp>   // the library + the command line tool class
```

Namespace `XYO::FileToRC` contains `using namespace XYO::System;`, so
`String`, `Shell::`, `IApplication`, ... are visible inside it.

Call the function qualified, `XYO::FileToRC::fileToRC(...)`. The metadata
namespaces exist in every XYO layer; here the library version is
`XYO::FileToRC::Version::version()` and the tool's is
`XYO::FileToRC::Application::Version::version()`.

## 5. First program

A tool that embeds every `.json` of a folder into one generated resource
script, one `RCDATA` per file:

```cpp
#include <XYO/FileToRC.hpp>

using namespace XYO::System;

class Application : public virtual IApplication {
		XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Application);

	public:
		inline Application(){};

		int main(int cmdN, char *cmdS[]);
};

int Application::main(int cmdN, char *cmdS[]) {
	TDynamicArray<String> fileList;
	Shell::getFileList("config/*.json", fileList);

	String output = "Config.Source.rc";
	Shell::removeFile(output);

	for (size_t k = 0; k < fileList.length(); ++k) {
		// config/default.json -> config_default
		String name = String("config_") + Shell::getFileBasename(Shell::getFileName(fileList[k]));
		// append = true: every call adds one more RCDATA line to the same file
		if (!XYO::FileToRC::fileToRC(name, fileList[k], output, true)) {
			printf("* Error: %s\n", fileList[k].value());
			return 1;
		};
	};
	return 0;
};

XYO_APPLICATION_MAIN(Application);
```

`XYO_APPLICATION_MAIN` comes from `xyo-system`: it initializes the managed
memory registry and calls `Application::main`. The resource name must not
contain spaces or quotes and is stored upper case (see
[Output format](output-format.md#resource-names)).

## 6. Conventions

- **`bool` means success.** `fileToRC` returns `false` when the input can
  not be read or the output can not be written; there are no exceptions
  and no message. The tool returns exit code `1`, also without a message
  (see [Command line](command-line.md#exit-codes)).
- **Paths are used as given**, relative to the current directory. The
  output folder must exist.
- **The output is overwritten** unless `append` / `--append` is used.
- **Names are not checked.** `--name` / `stringName` is written as is;
  no spaces, no quotes.
- **Any size compiles**, 16 words per line
  ([line length](output-format.md#line-length)); for files of several
  MB prefer `NAME RCDATA "file"` if the `.rc` need not be self-contained.
- **Odd sizes get one extra byte**, `0x0A`
  ([details](output-format.md#odd-sizes)).
- **Nothing is cached, no locking.** Do not write the same output file from
  two processes at the same time.

## 7. Building without fabricare

The tool is one translation unit plus the XYO layers. This is how
fabricare bootstraps itself (`build/fabricare.compile.json` in the
fabricare repository compiles `FileToRC.Application.Amalgam.cpp` into
fabricare):

1. Put `source/` of `xyo-platform`, `xyo-managed-memory`,
   `xyo-data-structures`, `xyo-multithreading`, `xyo-encoding`,
   `xyo-system` and `file-to-rc` on the include path, with the
   configuration headers of the layers that need them (see their
   documentation).
2. Define `XYO_PLATFORM_LIBRARY`, `XYO_MANAGEDMEMORY_LIBRARY`,
   `XYO_DATASTRUCTURES_LIBRARY`, `XYO_MULTITHREADING_LIBRARY`,
   `XYO_ENCODING_LIBRARY`, `XYO_SYSTEM_LIBRARY` and `XYO_FILETORC_LIBRARY`
   (static, no DLL export).
3. Compile `FileToRC.Application.Amalgam.cpp` with
   `Platform.Amalgam.cpp`, `ManagedMemory.Amalgam.cpp`,
   `DataStructures.Amalgam.cpp`, `Multithreading.Amalgam.cpp`,
   `Encoding.Amalgam.cpp` and `System.Amalgam.cpp` as C++17.

For the library only, compile `FileToRC.Amalgam.cpp` instead of
`FileToRC.Application.Amalgam.cpp`. To embed the tool without its `main`,
also define `XYO_FILETORC_APPLICATION_LIBRARY`.
