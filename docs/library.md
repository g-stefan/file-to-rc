# Library

```cpp
#include <XYO/FileToRC.hpp>

namespace XYO::FileToRC {
	bool fileToRC(const char *stringName, const char *fileNameIn, const char *fileNameOut, bool append);
};
```

The parameters are `const char *`; a `String` converts to it implicitly.
It returns `true` on success and `false` on any failure, prints nothing
and throws nothing. The exact output is in [Output format](output-format.md).

## fileToRC

```cpp
// logoPng RCDATA {\n\t0x5089,0x474E,...\n}\n
XYO::FileToRC::fileToRC("logoPng", "logo.png", "Logo.Source.rc", false);

// one more resource at the end of the same file
XYO::FileToRC::fileToRC("iconSave", "save.bin", "Logo.Source.rc", true);
```

| Parameter | Meaning |
|-----------|---------|
| `stringName` | resource name, written as given in front of `RCDATA` (an identifier, a number or a `#define`d id; no spaces) |
| `fileNameIn` | input, read as bytes |
| `fileNameOut` | output; its folder must exist |
| `append` | `false`: replace the output; `true`: add to its end (create it if missing) |

Returns `false` if the input can not be opened (the output is then not
touched) or the output can not be opened. Write errors after that (a full
disk) are not detected. It works with C `FILE *`, two bytes at a time, so
files of any size are converted without loading them into memory. The
output has 16 words per line, so `rc.exe` compiles it correctly at any
size (see [Output format](output-format.md#line-length)).

This is the function behind the command line tool.

## Regenerate only when changed

The library function always writes. To do what the tool's `--touch` does:

```cpp
if (!Shell::fileExists(out) || Shell::compareLastWriteTime(in, out) > 0) {
	if (!XYO::FileToRC::fileToRC(name, in, out, false)) {
		return false;
	};
	Shell::touchIfExists(includingRc);
};
```

## Embedding the command line tool

Depend on `file-to-rc.application.static` and call the tool class with an
argument vector, the way fabricare does:

```cpp
#include <XYO/FileToRC.Application.hpp>

int runFileToRC(TDynamicArray<String> &arguments) {
	int cmdN = (int)arguments.length() + 1;
	char **cmdS = new char *[cmdN];
	cmdS[0] = const_cast<char *>("file-to-rc");
	for (int k = 1; k < cmdN; ++k) {
		cmdS[k] = const_cast<char *>(arguments[k - 1].value());
	};
	int retV;
	{
		XYO::FileToRC::Application::Application application;
		retV = application.main(cmdN, cmdS);
	};
	delete[] cmdS;
	return retV;
};
```

`cmdS[0]` is the program name and is skipped. `TDynamicArray` is not
contiguous memory, so build a plain `char *` array. The tool prints to
`stdout` and returns the exit code; it never calls `exit`. The options are
in [Command line](command-line.md).

## Library metadata

As in every XYO library (the headers are not included by
`<XYO/FileToRC.hpp>`):

```cpp
#include <XYO/FileToRC/Version.hpp>
#include <XYO/FileToRC/Copyright.hpp>
#include <XYO/FileToRC/License.hpp>

XYO::FileToRC::Version::version();            // "5.9.0"
XYO::FileToRC::Version::build();
XYO::FileToRC::Version::versionWithBuild();
XYO::FileToRC::Version::datetime();
XYO::FileToRC::Copyright::copyright();
XYO::FileToRC::License::license();            // std::string
```

The tool has the same set in `XYO::FileToRC::Application::Version`,
`Copyright` and `License`.
