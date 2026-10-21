# Compiler reference

`nanoc` is an experimental x86-64 C compiler written in
Neutron. It preprocesses C source,
lowers it to AT&T assembly, and assembles relocatable ELF objects itself.
Executable linking uses the host C toolchain by default; an experimental
opt-in native ELF linker is also available.

## Requirements

- Linux x86-64
- `arq`
- `ntsc` on `PATH`
- A host C toolchain providing `cc` for default executable linking
- glibc and its x86-64 dynamic loader for `--native-link`

Tests additionally use `gcc`, `ar`, `readelf`, `objcopy`, `mktemp`, and a POSIX
shell.
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
# Position-independent executable, still linked entirely natively
./build/debug/nanoc --native-link -pie -o output input.c
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
`ET_EXEC` executable by default, or an `ET_DYN` position-independent executable
with `-pie`. It generates its own startup code, initializes
via glibc's `__libc_start_main`, and emits dynamic symbols, a SysV hash table,
GOT-backed function stubs, and eager dynamic relocations. The executable has
separate RX/RW load segments and a non-executable stack.

libc is loaded automatically. `-lm`, `-L dir -lname`, and explicit ELF `.so`
inputs support function and ordinary data imports; shared-library paths are recorded
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
`R_X86_64_PC32`, `R_X86_64_PLT32`, `R_X86_64_32`, `R_X86_64_32S`,
`R_X86_64_GOTPCREL`, `R_X86_64_GOTPCRELX`, `R_X86_64_REX_GOTPCRELX`,
and `R_X86_64_GOTPC32`, with bounds and 32-bit overflow checks. Unsupported
relocations fail explicitly; the native linker **never falls back** to the host
toolchain.

PIC and `-fno-plt` host objects using these GOT relocations are supported,
including archive members. The linker allocates real GOT slots without
instruction relaxation; local definitions are resolved at link time, while
imported function pointers consistently use the generated callable stub.

Absolute (`SHN_ABS`) symbols and global common (`SHN_COMMON`) object symbols
from ELF inputs are supported. Common declarations combine their maximum
size and alignment, are zero-initialized at final layout, and yield to a
strong definition regardless of object order. Strong definitions in archives
can also replace common declarations. The current implementation materializes
common storage in `.data` and limits it to 16 MiB with alignment up to 4096.

Weak ELF definitions yield to common and strong definitions, with the first
weak definition retained when no stronger one exists. An unresolved weak
reference becomes zero and does not trigger archive extraction; a strong
reference can still pull a weak provider out of an archive. C declarations
can use `__attribute__((weak))` before the type or after the declarator.
Weak declarations require external linkage.

Ordinary shared-library data imports, including libc's `stdin`, `stdout`,
`stderr`, and `optind`, use `R_X86_64_COPY` relocations. The executable reserves
aligned storage and exports its symbols so both native references and
preemptible shared-library references see the same data. Function-pointer
and data-pointer initializers, PIC GOT references, arrays, and executable data
overrides are tested. Fixed-address executables retain the copy-relocation path
for preemptible ordinary data. Protected data imports and all PIE data imports
instead use runtime GOT and 64-bit pointer relocations, preserving the shared
library's storage and address identity without copies. The compiler now emits
GOT-relative global addresses by default. Host objects that access protected
imports through direct PC-relative memory relocations must be rebuilt with
`-fPIC`; they are rejected rather than incorrectly copied. Zero-sized data
imports and imports larger than 16 MiB still fail explicitly.

Constructor/destructor functions can use
`__attribute__((constructor))` / `__attribute__((destructor))`, optionally with
an integer priority from 101 to 65535. nanoc requires these functions to return
`void` and take no parameters. Attributes on an earlier prototype carry through
to its definition. Native `.o` files retain numbered `.init_array`/`.fini_array`
sections for host-linker interoperability. The native executable emits the
corresponding dynamic tags, sorts priorities in increasing order for
initialization, and lets glibc run finalizers in reverse order on normal return
or `exit`. `_Exit` intentionally bypasses finalizers. Allocated host
`.preinit_array` inputs are also supported. Archive members run their lifecycle
functions only when selected.

Legacy `.ctors`/`.dtors` lists and `.init`/`.fini` code fragments are supported
from host ELF objects. Legacy priorities are translated to initialization
priorities, constructors run in reverse list order, and destructors in forward
list order. Zero/-1 list sentinels are removed. Concatenated `.init`/`.fini`
fragments are emitted as `DT_INIT`/`DT_FINI` routines with a trailing return when
needed. As with modern arrays, malformed arrays and non-code function pointers
are rejected. Unsupported declaration attributes fail explicitly; only `weak`,
`constructor`, and `destructor` (plus their double-underscore aliases) are
implemented.

### PIE and thread-local storage

`--native-link -pie` emits loader metadata with `DF_1_PIE`, relative load-bias
relocations for local addresses, and dynamic relocations for imported addresses.
Native C objects, static pointer initializers, lifecycle arrays, and compatible
PIC host objects/archives are tested. `-no-pie` overrides `-pie`; the default
remains a fixed-address executable. `-fPIC`/`-fPIE` spellings are accepted because
the native global-address path is already GOT-relative. Writable/executable
segments and runtime text relocations are not introduced. Non-PIC 32-bit absolute
addresses and PC-relative references to absolute symbols in PIE inputs still
produce explicit errors rather than truncated or incorrectly relocated output.

C `__thread` and `_Thread_local` declarations emit an ELF TLS template and
`STT_TLS` symbols. File-scope initialized/zero storage and block-scope `static`
or `extern` TLS are supported; automatic block-scope TLS and constant static
initializers containing a TLS address are rejected. Native access uses the
global-dynamic model with `__tls_get_addr`. The linker also accepts host
objects using global-dynamic, local-dynamic, initial-exec, local-exec, and
TLSDESC models without host-linker relaxation. Shared-library TLS uses its
provider module; local-exec and static offset relocations require definitions
in the executable. A `PT_TLS` header, TLS GOT descriptors, and TLS dynamic
relocations let glibc allocate independent storage for every thread. Tests
exercise initialized/zero templates, pthread isolation, native/archive objects,
shared imports, and pointer relocations in TLS templates, for PIE and non-PIE.
TLS storage is currently materialized in the file, including zero-fill bytes,
with the same 16 MiB per-section and 4096-byte alignment limits as other native
inputs. Malformed/mismatched TLS symbols and descriptor calls fail explicitly.
Unresolved weak TLS imports and raw input `DTPMOD64` relocations are not supported.

Large-model GOT relocations, non-default visibility in object inputs, linker
scripts, and `-Wl` flags remain unsupported. PIE and TLS support is a tested
subset, not full GNU linker parity. Native linking stays opt-in and never
silently falls back to `cc`, `as`, `ld`, or `ar`.

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
`arq test_native`, `arq test_archive`, `arq test_got`, `arq test_symbols`,
`arq test_weak`, `arq test_data_imports`, `arq test_lifecycle`, `arq test_pie`,
`arq test_protected`, `arq test_tls`, and `arq test_legacy`.
Each depends on `build`; a full invocation builds once before the isolated test
tasks run. The native suite hides the host toolchain with an empty `PATH`,
checks ELF metadata, compares native and host-linked output, and exercises
multiple units, shared-library imports, `-run`, scalar/aggregate ABI at `-O0`
and `-O2`, and explicit failures for unsupported relocations and inputs.
The archive suite also hides `ar` from nanoc and checks lazy member selection,
dependency closure, input ordering, long names, padding, and malformed inputs.
The GOT suite checks PIC/no-PLT and non-relaxable relocations at `-O0`/`-O2`,
archive members, private locals, function-pointer identity, arbitrary addends,
and overflow/bounds rejection, again with the toolchain hidden from nanoc.
The symbol suite covers absolute values, common storage, strong overrides,
archive resolution, alignment/size bounds, and duplicate definitions.
The feature suites additionally check weak resolution, libc/shared data and
copy relocations, executable exports, lifecycle priorities, host-object
interoperability, preinit, normal/early exit, and shared-library initialization.
The gap-regression suites cover PIE/load-bias metadata and execution, protected
data identity/mutations, all five tested TLS access models with real pthreads,
TLS imports/templates, and legacy lifecycle routines. These native paths run
with an empty `PATH`, so `cc`, `as`, `ld`, and `ar` cannot be used as a fallback.

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
- GNU-style `weak`, `constructor`, and `destructor` declaration attributes
  with native object/linker support
- C `__thread` / `_Thread_local` storage with native ELF TLS templates and
  global-dynamic access

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
- Large-model GOT relocations, weak TLS imports, and broader host-object
  compatibility
- Broader symbol visibility/versioning and dynamic-linker compatibility
- Additional ELF validation and malformed-input hardening
- More portable shared-library lookup/deployment and broader PIE compatibility

`--native-link` will remain opt-in while these paths are implemented and
verified. Run `arq test_native` to check the current no-toolchain path.

## License

MIT. See [LICENSE](../LICENSE).
