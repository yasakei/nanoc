# nanoc

`nanoc` is an experimental x86-64 C compiler written in
[Neutron](https://github.com/yasakei/neutron-rs). It preprocesses C source,
lowers it to AT&T assembly, and assembles relocatable ELF objects itself.
Executable linking uses the host C toolchain by default; an experimental
opt-in native ELF linker is also available.

## Requirements

- Linux x86-64
- `arq`
- `ntsc` on `PATH`
- A host C toolchain providing `cc` for default executable linking
- glibc and its x86-64 dynamic loader for `--native-link`

Tests additionally use `gcc`, `ar`, `readelf`, `mktemp`, and a POSIX shell.
Test orchestration and assertions are written in Arq; Python is not required.

## Build

```sh
arq build
```

The compiler is written to `build/debug/nanoc`.

## Usage

```sh
# Preprocess
./build/debug/nanoc -E input.c

# Emit assembly
./build/debug/nanoc -S -o output.s input.c

# Compile an ELF object (native assembler; no host toolchain needed)
./build/debug/nanoc -c -o input.o input.c

# Compile and link
./build/debug/nanoc -o output input.c other.c library.o
./output

# Include paths, macro definitions, and library linking
./build/debug/nanoc -I include -D FEATURE=1 -U OLD_FEATURE input.c -L lib -lm

# Native ELF linking (experimental; no cc, as, or ld)
./build/debug/nanoc --native-link -o output input.c other.c library.o
./build/debug/nanoc --native-link -lm -o output input.c
./build/debug/nanoc --native-link input.c -L lib -lanswer -o output

# Compile and execute, forwarding arguments and the program's exit status
./build/debug/nanoc -run input.c "argument with spaces"
./build/debug/nanoc --native-link -run input.c "argument with spaces"
```

Without `-o`, `-c` and `-S` produce basename-based `.o` and `.s` files.
`-E -o output.i` writes preprocessed source without parsing it as C.
`-run` uses temporary output and does not overwrite `a.out`. Object assembly
is native, without invoking `cc`, `as`, or `ld`. Final linking still uses `cc`
unless `--native-link` is supplied. Put that option before the source filename
when combining it with `-run`, since subsequent arguments go to the program.

### Experimental native linker

`--native-link` merges C translation units and ELF64 x86-64 `.o` files into a
non-PIE `ET_EXEC` executable. It generates its own startup code, initializes
via glibc's `__libc_start_main`, and emits dynamic symbols, a SysV hash table,
GOT-backed function stubs, and eager dynamic relocations. The executable has
separate RX/RW load segments and a non-executable stack.

libc is loaded automatically. `-lm`, `-L dir -lname`, and explicit ELF `.so`
inputs are supported for function imports; shared-library paths are recorded
as absolute `DT_NEEDED` entries, so moving/removing those libraries requires
relinking. This is not yet a portable deployment format.

Static `.a` archives are read directly, without invoking `ar`. GNU short/long
member names and BSD extended names are supported. The linker loads only
members needed to resolve undefined symbols, repeating selection within each
archive to include its dependencies. Archives are searched at their position
on the command line: put `library.a` or `-lname` **after** its users, and repeat
an archive if a later archive introduces a dependency back into it.
`-L dir -lname` prefers a shared library over an archive within each searched
directory. Archive indexes are optional; member ELF symbol tables drive
selection.

Thin archives, nested archives, non-ELF members, archive creation, and
`--whole-archive`/archive groups are not implemented. Unsupported relocations
in an unused member do not prevent linking; selected members are validated
fully. Native input files, including archives, are limited to 16 MiB.

Supported object relocations are `R_X86_64_NONE`, `R_X86_64_64`,
`R_X86_64_PC32`, `R_X86_64_PLT32`, `R_X86_64_32`, and `R_X86_64_32S`, with
bounds and 32-bit overflow checks. Unsupported relocations fail explicitly;
the native linker **never falls back** to the host toolchain.

PIE, GOT/TLS object relocations, data imports (including `stdout`
and `optind`), weak/common/absolute symbols, non-default visibility, linker
scripts, `-Wl` flags, and constructor/destructor sections are not supported.
Host objects with GOT relocations may need `-fno-pic -fno-pie`; this does not
guarantee that every host object is supported. Native linking stays opt-in
while this limited surface is verified.

## Tests

Run the complete suite with:

```sh
arq test
```

The Arq runners in `tests/arq/` cover preprocessing against `gcc`, driver
options, multiple translation units, local symbol linkage, shell quoting,
standard I/O headers, variadic functions, function pointers, integer widths,
and bidirectional scalar/aggregate ABI interoperability with host-compiled C
functions at `-O0` and `-O2`. Language tests also cover 64-bit arithmetic, mixed
integer/floating expressions, assignment expressions, pointer math,
declarations, initializers, and persistent block-scope static storage against
GCC. Rejection tests check invalid bounds, excess initializers, unknown members,
and incompatible assignments.
They create isolated directories under `build/`, remove them after success,
and retain failed-test artifacts for inspection.

Individual suites can be run with `arq test_preprocessor`, `arq test_driver`,
`arq test_abi`, `arq test_smoke`, `arq test_language`, `arq test_rejections`,
`arq test_native`, and `arq test_archive`.
Each depends on `build`; a full invocation builds once before the isolated test
tasks run. The native suite hides the host toolchain with an empty `PATH`,
checks ELF metadata, compares native and host-linked output, and exercises
multiple units, shared-library imports, `-run`, scalar/aggregate ABI at `-O0`
and `-O2`, and explicit failures for unsupported relocations and inputs.
The archive suite also hides `ar` from nanoc and checks lazy member selection,
dependency closure, input ordering, long names, padding, and malformed inputs.

Optionally compare the language and aggregate ABI tests with an installed TinyCC:

```sh
TCC=/path/to/tcc arq test_language
TCC=/path/to/tcc arq test_abi
# For an uninstalled checkout, also supply its internal library directory:
TCC=/path/to/tinycc/tcc TCC_LIB=/path/to/tinycc arq test
```

The scalar-brace trailing-comma regression uses GCC alone because the tested
TinyCC checkout rejects that C11 syntax.

The optional ABI checks link TinyCC objects with TinyCC so its runtime helpers
are included. They run the full aggregate fixture as a TinyCC-only reference
and cross-link homogeneous integer/SSE records and large records with nanoc.
The tested TinyCC checkout uses a different convention for mixed integer/SSE
records; nanoc follows the per-eightbyte System V convention verified with GCC,
so mixed-record TinyCC objects are not advertised as interoperable.

## Supported surface

- C preprocessing, includes, conditionals, variadic macros, stringization,
  and token pasting
- Integers, floating point, pointers, arrays, functions, typedefs, enums,
  structs, unions, and basic bitfields
- Arithmetic, casts, assignments, control flow, `goto`, labels, and function
  pointers
- `stdio.h`, `stdarg.h`, `stdint.h`, `stdatomic.h`, `math.h`, and related
  compatibility headers
- Separate integer and floating-point argument registers, scalar stack-passed
  arguments, nested-call stack alignment, and correctly sized float storage
- Integer/pointer/double variadic calls, `va_arg`, `va_copy`, register exhaustion,
  and forwarding `va_list` to host functions
- Local stack frames sized from declarations instead of a fixed frame limit
- Typed 32/64-bit signed and unsigned arithmetic, promotions, conversions,
  pointer differences, compound assignments, and prefix/postfix increments
- Mixed integer/floating expressions and unordered (NaN) comparisons
- x87 long-double arithmetic, comparisons, conversions, 16-byte storage,
  stack arguments, return values, and variadic arguments
- Multiple declarators, local typedefs and shadowing, multidimensional arrays,
  pointer-to-array declarators, inferred outer array bounds, and unevaluated
  `sizeof` operands
- Typed local/global array, struct, and union initializers, zero-filled omitted
  elements, simple array/member designators, character-array strings with
  decoded escapes, and compatible struct/union copy assignments
- Persistent block-scope `static` scalars, arrays, structs, function pointers,
  and addresses, with private labels and independent nested-scope storage
- x86-64 System V struct/union arguments and returns: integer/SSE register
  classification, mixed-class records, nested records and array members,
  stack fallback when registers are exhausted, and hidden return buffers for
  large records
- Aggregate `va_arg`, named aggregate variadic parameters, variadic aggregate
  returns, and direct/indirect calls interoperating with host C

## Limitations

The project is experimental and does not yet provide full TinyCC parity.
It targets x86-64 Linux. The default executable path relies on the host
assembler/linker and libc; `-c` is native and `--native-link` removes the host
assembler/linker dependency with the restrictions above.
Broader floating-point expressions, diagnostics, and standards coverage
remain incomplete; the tested long-double subset is not full standards parity.
The aggregate ABI does not yet cover bitfields, packed/explicitly aligned
records, vectors, or complex numbers.
Variable-length arrays, bitfield initializers,
brace-elided multidimensional initializers, chained designators, and general
static floating-point constant expressions are not implemented.
In-memory compilation, a libtcc-compatible API, cross-target backends, and
C++ language features are not supported.

## Next steps

The native backend is being extended incrementally, with rejection tests for
unsupported inputs and no implicit host-toolchain fallback. Remaining work
includes:

- Thin archives, archive groups, and archive creation
- GOT relocations and broader host-object compatibility
- Weak/common/absolute symbols and data imports
- Constructor/destructor handling and additional ELF validation
- PIE support and more portable shared-library lookup/deployment

`--native-link` will remain opt-in while these paths are implemented and
verified. Run `arq test_native` to check the current no-toolchain path.

## License

MIT. See [LICENSE](LICENSE).
