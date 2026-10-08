# API reference

## Headers

| Header | Contents |
|--------|----------|
| `<XYO/FileToRC.hpp>` | the library: `Dependency.hpp` + `Library.hpp` |
| `<XYO/FileToRC/Copyright.hpp>`, `License.hpp`, `Version.hpp` | library metadata (not included by `FileToRC.hpp`) |
| `<XYO/FileToRC.Application.hpp>` | the library + `XYO::FileToRC::Application::Application` |

## Macros

| Macro | Meaning |
|-------|---------|
| `XYO_FILETORC_EXPORT` | `dllexport` / `dllimport` / nothing |
| `XYO_FILETORC_DLL_INTERNAL` | building the DLL (exports); set from `FILE_TO_RC_DLL_INTERNAL`, which xyo-cc defines from the project name |
| `XYO_FILETORC_INTERNAL` | set from `FILE_TO_RC_INTERNAL`; not used by the sources |
| `XYO_FILETORC_LIBRARY` | static use, `XYO_FILETORC_EXPORT` is empty (set by `file-to-rc.static`) |
| `XYO_FILETORC_APPLICATION_LIBRARY` | build the tool without `main` (set by `file-to-rc.application.static`) |

## namespace XYO::FileToRC

`using namespace XYO::System;`

| Function | Writes | Returns |
|----------|--------|---------|
| `bool fileToRC(const char *stringName, const char *fileNameIn, const char *fileNameOut, bool append)` | `name RCDATA {\n\t0xHHHH,...\n}\n`, 16 words per line, two input bytes per word, low byte first, an odd last byte padded with `0x0A`; appends if `append` | `false` if input or output can not be opened |

Details and limits: [Output format](output-format.md).

## namespace XYO::FileToRC::Version / Copyright / License

```cpp
const char *XYO::FileToRC::Version::version();
const char *XYO::FileToRC::Version::build();
const char *XYO::FileToRC::Version::versionWithBuild();
const char *XYO::FileToRC::Version::datetime();

const char *XYO::FileToRC::Copyright::copyright();
const char *XYO::FileToRC::Copyright::publisher();
const char *XYO::FileToRC::Copyright::company();
const char *XYO::FileToRC::Copyright::contact();

std::string XYO::FileToRC::License::license();
std::string XYO::FileToRC::License::shortLicense();
```

The same functions exist for the tool in
`XYO::FileToRC::Application::Version`, `Copyright` and `License`.

## class XYO::FileToRC::Application::Application

```cpp
class Application : public virtual IApplication {
	public:
		void showUsage();
		void showLicense();
		void showVersion();
		int main(int cmdN, char *cmdS[]);
		static void initMemory();
};
```

The `file-to-rc` command line tool; `main` returns the exit code. See
[Command line](command-line.md).

## Command line options

`--help`, `--usage`, `--license`, `--version`, `--name=name`,
`--file-in=file`, `--file-out=file`, `--append`, `--touch=file`, `@file`.

| Exit code | Meaning |
|-----------|---------|
| `0` | written, up to date (`--touch`), or an info option |
| `1` | missing / empty option, response file not found, read or write failed (no message for the last) |

## fabricare projects

| Project | Kind | Defines exported to the consumer |
|---------|------|----------------------------------|
| `file-to-rc` | DLL / shared library, version key `file-to-rc.library` | |
| `file-to-rc.static` | static library, static CRT | `XYO_FILETORC_LIBRARY` |
| `file-to-rc.application.static` | static library, tool without `main` | `XYO_FILETORC_APPLICATION_LIBRARY` |
| `file-to-rc` | executable | |
