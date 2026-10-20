# nanoc

`nanoc` is an experimental x86-64 C compiler written in
[Neutron](https://github.com/yasakei/neutron-rs). It preprocesses C source,
lowers it to AT&T assembly, and uses the host C toolchain for final linking.

## Requirements

- Linux x86-64
- `arq`
- `ntsc` on `PATH`
- A host C toolchain providing `cc`

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

# Compile and link
./build/debug/nanoc -o output input.c
./output
```

## Tests

Run the complete smoke suite with:

```sh
arq test
```

The suite covers preprocessing against `gcc`, standard I/O headers, bundled
headers, variadic functions, function pointers, and integer width/sign rules.
Test inputs are under `tests/`.

## Supported surface

- C preprocessing, includes, conditionals, variadic macros, stringization,
  and token pasting
- Integers, floating point, pointers, arrays, functions, typedefs, enums,
  structs, unions, and basic bitfields
- Arithmetic, casts, assignments, control flow, `goto`, labels, and function
  pointers
- `stdio.h`, `stdarg.h`, `stdint.h`, `stdatomic.h`, `math.h`, and related
  compatibility headers
- Integer/pointer variadic calls and `va_arg`

## Limitations

The project is experimental. It currently targets x86-64 Linux, emits assembly
instead of native object files, and relies on the host libc. Floating-point
variadic ABI coverage, stack-passed arguments, diagnostics, and standards
coverage are still incomplete. C++ headers and C++ language features are not
supported.

## License

MIT. See [LICENSE](LICENSE).
