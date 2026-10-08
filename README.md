# File to RC Source

Utility and C++ library to convert a file to RC source
- Embeds any file in a Windows executable or DLL as an inline `RCDATA`
resource: `name RCDATA { 0x4241,0x4443,... }`, read at run time with
`FindResource` / `LoadResource`.
- `file-to-rc` command line tool: `--name`, `--file-in`, `--file-out`,
`--append` (several resources in one file), `--touch` (regenerate only when
the input changed, then touch the `.rc` that includes it), `@file` response
files.
- Library `XYO::FileToRC`: `fileToRC`, and the tool as a class to embed in
other programs.

Built on `xyo-system`; linked into `fabricare`, where build scripts call it
as `fileToRC(...)`.

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - build, depend on it, use it in a build, first program
- [Command line](docs/command-line.md) - `file-to-rc` options, `--touch`, `--append`, exit codes, examples
- [Output format](docs/output-format.md) - exact output, byte order, odd sizes, line length, reading the resource, pitfalls
- [Library](docs/library.md) - `fileToRC`, embedding the tool
- [API reference](docs/reference.md)

A Claude Code skill for this library is in
[.claude/skills/file-to-rc](.claude/skills/file-to-rc/SKILL.md).

## License

Copyright (c) 2007-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
