# nanoc

`nanoc` is an experimental C compiler written in [Neutron](https://github.com/yasakei/neutron-rs).
It is deliberately small, direct, and hackable: source is preprocessed, parsed
into a compact type model, lowered to x86-64 AT&T assembly, and handed to the
host C toolchain for assembly and linking.

The project is aimed at learning and iteration rather than standards compliance.
The implementation favors a clear compiler pipeline and executable examples over
a large framework.

## What works

- C preprocessing:
  - object-like and function-like macros
  - variadic macros, stringization, and token pasting
  - `#include`, conditional compilation, `defined`, and `__LINE__`
  - project headers, bundled headers, and host system headers
- C declarations and types:
  - integers, floating-point values, pointers, arrays, functions, and typedefs
  - structs, unions, enums, anonymous records, and basic bitfields
  - qualifiers and fixed-width integer aliases
- Expressions and statements:
  - arithmetic, comparisons, casts, pointer arithmetic, and assignments
  - short-circuit logic and the conditional operator
  - `if`, loops, `switch`, `break`, `continue`, `goto`, and labels
  - function pointers and indirect calls
- Runtime-facing headers:
  - standard I/O declarations in `include/stdio.h`
  - variadic functions through `stdarg.h`
  - integer, floating-point, atomic, boolean, alignment, and math headers
- x86-64 System V integer/pointer variadic calls and `va_arg` support

## Requirements

- Linux x86-64
- Arq on `PATH`
- A working Neutron compiler (`ntsc`) on `PATH`
- A host C toolchain with `cc`
- `git` for source checkout and development

The Neutron compiler is currently built from the sibling project at
`../neutron-rs` in the development environment. Install or expose `ntsc`
yourself when using a different checkout layout.

## Build

```sh
arq build
```

The compiler binary is written to:

```text
build/debug/nanoc
```

## Usage

Preprocess a source file:

```sh
./build/debug/nanoc -E input.c
```

Emit assembly:

```sh
./build/debug/nanoc -S -o output.s input.c
```

Compile and link an executable:

```sh
./build/debug/nanoc -o output input.c
./output
```

For example:

```sh
./build/debug/nanoc -o /tmp/nanoc-stdio tests/c/stdio.c
/tmp/nanoc-stdio
```

## Checks

The repository includes a repeatable Arq test task for the current compiler
surface:

```sh
arq build
arq test
```

`arq test` builds nanoc, compares the preprocessor with the host compiler, and
runs the stdio, bundled-header, variadic, and integer-width programs.

The preprocessor comparison can also be run directly:

```sh
python3 tests/preprocessor_compare.py
```

The generated compiler output should be normalized before comparing because
`nanoc -E` retains source-location markers.

## Repository layout

```text
src/                 compiler implementation
include/             bundled C compatibility headers
tests/c/              C compiler smoke programs
tests/preprocessor/   preprocessor regression input and fixture header
tests/preprocessor_compare.py  differential preprocessor check
build.arq             build and test tasks
neutron.toml         Neutron package configuration
```

The compiler is organized as a straightforward pipeline:

1. `src/lexer.nt` tokenizes C source.
2. `src/pp.nt` expands directives and macros.
3. `src/parse.nt` builds declarations, types, and layout information.
4. `src/ctype.nt` contains the shared canonical type model.
5. `src/codegen.nt` emits x86-64 assembly.
6. `src/main.nt` provides the command-line driver.

## Header and library model

`nanoc` supplies declarations and compatibility headers; it does not replace
the host C library. Final linking uses the system `cc`, so functions such as
`printf`, `puts`, and `sprintf` come from the platform libc.

The project includes a broad standard/POSIX-oriented `stdio.h` declaration
surface and a small math header so ordinary C programs can use the host
runtime without importing compiler-specific implementation details.

C++ headers and C++ language features are outside the scope of this compiler.

## Current limitations

This is an active compiler project, not a production C implementation. Known
limitations include:

- x86-64 Linux is the supported target.
- Assembly is linked through the host toolchain; nanoc does not yet emit native
  ELF object files directly.
- Variadic `va_arg` currently focuses on integer and pointer values.
- Floating-point ABI coverage and stack-overflow argument handling are still
  being expanded.
- The C library surface depends on the host platform and linker.
- Diagnostics and standards coverage are intentionally incomplete.

## Contributing

Keep changes focused, add a small executable or preprocessing regression when
possible, and run `arq check` before opening a pull request. Avoid committing
generated files from `build/` or generated assembly outputs.

## License

nanoc is distributed under the MIT License. See [LICENSE](LICENSE).

Some compatibility headers retain their own upstream license notices where
applicable.
