# Command line

```
file-to-rc [options] [@file ...]
```

## Options

| Option | Effect |
|--------|--------|
| `--help`, `--usage` | print usage, exit 0 |
| `--license` | print the license, exit 0 |
| `--version` | print the tool's own version, exit 0 |
| `--name=name` | name of the `RCDATA` resource; required |
| `--file-in=file` | input file, any content; required |
| `--file-out=file` | output `.rc` file; required; its folder must exist |
| `--append` | add the resource to the end of the output instead of replacing it |
| `--touch=file` | regenerate only if the input is newer than the output; afterwards touch `file` |
| `@file` | read more arguments from `file` (a response file) |

`--help`, `--usage`, `--license` and `--version` act as soon as they are
seen. Arguments that do not start with `--` (other than `@file`) and
unknown options are ignored. An empty `--name=`, `--file-in=`,
`--file-out=` or `--touch=` is an error. When a required option is
missing, the usage is printed and the exit code is `1`.

The exact output is in [Output format](output-format.md).

## Order of work

1. Expand the `@file` response files, then read the options.
2. Check `--name`, `--file-in`, `--file-out`.
3. `--touch`: if the output exists and the input is **not newer** than it,
   stop with exit code `0`; nothing is written or touched.
4. Write the output, replacing it or (`--append`) appending to it.
5. `--touch`: touch the file if it exists (a missing one is not an error).

## --touch

The generated file is `#include`d by a hand written `.rc`. A build tool
that checks only the time of the `.rc` will not see a change in the
generated file, and regenerating on every build changes its time and
recompiles it every time. `--touch=Application.rc` handles both:

- input not newer than output → nothing happens, the build stays
  incremental;
- input newer, or no output yet → the output is written and
  `Application.rc` gets the current time, so the resources are recompiled.

The check compares the last write time of `--file-in` and `--file-out`
(`Shell::compareLastWriteTime`, nanoseconds on Linux).

Do not combine `--touch` with `--append` on a shared output: once the
first input has written the file, the next ones are compared against it
and skipped.

## --append

```
file-to-rc --name=iconOpen --file-in=open.bin --file-out=Data.Source.rc
file-to-rc --name=iconSave --file-in=save.bin --file-out=Data.Source.rc --append
file-to-rc --name=iconExit --file-in=exit.bin --file-out=Data.Source.rc --append
```

The first call replaces the output, the next ones add one resource each. Every
run of an `--append` call adds again: start the sequence with a call
without `--append` (or remove the output first), or the file collects
duplicate resources. `rc.exe` accepts them; the link fails later with
`CVTRES : fatal error CVT1100: duplicate resource`.

## Exit codes

| Code | Meaning |
|------|---------|
| `0` | written; also up to date (`--touch`), and `--help` / `--usage` / `--license` / `--version` |
| `1` | missing or empty required option, response file not found, input can not be read, output can not be written |

Messages go to `stdout`. Option errors print `Error: name is empty`,
`Error: file-in is empty`, `Error: file-out is empty`,
`Error: touch filename is empty`, or `Error: file not found - <file>` for a
response file. A failed conversion (input missing, output folder missing,
no access) prints **nothing**: only the exit code says it failed. Always
check it.

## Response files

`@file` is replaced by the arguments read from `file`, split like a command
line (quotes group words). Several `@file` and normal arguments can be
mixed:

```
--name=defaultConfig
--file-in=DefaultConfig.json
"--file-out=DefaultConfig.Source.rc"
```

```
file-to-rc @config.args --touch=Application.rc
```

## From fabricare

Inside fabricare the tool is linked in (`file-to-rc.application.static`)
and is available to scripts as the function `fileToRC(...)`, which takes
the same arguments as strings, one per parameter, and returns the exit
code:

```js
runInPath("source/XYO/MyTool", function() {
	exitIf(fileToRC("--touch=Application.rc", "--name=defaultConfig", "--file-in=DefaultConfig.json", "--file-out=DefaultConfig.Source.rc"));
});
```

The installed executable is on the `PATH` of the build as well:

```js
exitIf(Shell.system("file-to-rc --touch=Application.rc --name=defaultConfig --file-in=DefaultConfig.json --file-out=DefaultConfig.Source.rc"));
```

## Examples

A file, as an `RCDATA` resource named `LOGO`:

```
file-to-rc --name=LOGO --file-in=logo.bin --file-out=Logo.Source.rc
```

With a numeric id defined in a header that the `.rc` includes before the
generated file (`#define IDR_LOGO 101`):

```
file-to-rc --name=IDR_LOGO --file-in=logo.bin --file-out=Logo.Source.rc
```

Only when the input changed:

```
file-to-rc --touch=Application.rc --name=LOGO --file-in=logo.bin --file-out=Logo.Source.rc
```
