# Compiler Construction Labs

This repository contains the BMSTU IU9 sixth-semester compiler-construction
assignments completed in 2024. The projects form a progression from lexers to
parser generation, AST transformations, formatting, and semantic analysis.
Most implementations use C++20; two exercises use Python and one uses Go.

## Highlights

- [`lab3-1`](lab3-1/README.md) is a self-applicable LL(1) parser-table
  generator with semantic grammar validation and a calculator demo.
- [`lab3-2`](lab3-2/README.md) combines Flex, Bison, an AST, an identifier
  table, and a Visitor-based source formatter.
- [`lab2-4`](lab2-4/README.md) is a hand-written recursive-descent parser that
  serializes a functional-language AST as JSON.

## Course progression

| Lab | Topic and result | Implementation | Language/tooling | Report |
|---|---|---|---|---|
| [`lab1-1`](lab1-1/) | Bootstrapping a self-applicable P5 Pascal compiler; out-of-range integer constants become zero | Modified P5 compiler | Pascal/P5 | [report](lab1-1/report.md) |
| [`lab1-2`](lab1-2/) | Regex-based lexer for custom string, numeric, and identifier tokens | Regular-expression scanner | C++20, Boost.Regex | [report](lab1-2/report.md) |
| [`lab1-3`](lab1-3/) | Hand-written object-oriented lexer for numeric and regular/verbatim string literals | Stateful scanner and token hierarchy | C++20 | [report](lab1-3/report.md) |
| [`lab1-4`](lab1-4/) | Lexer built from a labeled DFA; NFA/DFA diagrams are included | Table-driven scanner | C++20 | [report](lab1-4/report.md) |
| [`lab1-5`](lab1-5/) | Flex-generated lexer for chemical equations | Generated scanner | C++20, Flex | [report](lab1-5/report.md) |
| [`lab2-1`](lab2-1/) | Go AST transformation that instruments loops to count total iterations | Source-to-source AST rewrite | Go AST packages | [report](lab2-1/report.md) |
| [`lab2-2`](lab2-2/) | Grammar and AST design for a typed functional language with pattern matching | Parser EDSL grammar | Python, parser EDSL | [report](lab2-2/report/report.md) |
| [`lab2-3`](lab2-3/) | Predictive parser for a grammar-description language | Table-driven LL(1) parser | C++20, Flex | [report](lab2-3/report/report.md) |
| [`lab2-4`](lab2-4/) | Recursive-descent parser for the functional language; emits an AST as JSON | Flex tokenizer and recursive descent | C++20, Flex, Boost.JSON | [report](lab2-4/report/report.md) |
| [`lab3-1`](lab3-1/) | Self-applicable LL(1) parser-table generator: parse tree -> AST -> validation -> FIRST/FOLLOW -> generated C++ table; calculator demo | Generator and generated predictive parsers | C++20, Flex, Boost | [report](lab3-1/report/report.md) |
| [`lab3-2`](lab3-2/) | Source formatter for the functional language using a generated parser and Visitor traversal | Generated frontend and formatter | C++20, Flex, Bison | [report](lab3-2/report/report.md) |
| [`lab3-3`](lab3-3/) | Semantic analysis and type checking for the functional language | Parser EDSL and semantic passes | Python, parser EDSL | [report](lab3-3/report/report.md) |

## Quick start

Run the primary showcase from the repository root:

```sh
cmake -S lab3-1/apps/generator/src -B build/generator
cmake --build build/generator
./build/generator/generator \
  --grammar lab3-1/apps/generator/examples/arithmetic_grammar.txt \
  --template lab3-1/apps/generator/templates/analyzer_table.cc \
  --table build/arithmetic_table.cc

cmake -S lab3-1/apps/calculator/src -B build/calculator
cmake --build build/calculator
./build/calculator/calculator lab3-1/apps/calculator/examples/example.txt
# 15
```

Then build and run the formatter:

```sh
cmake -S lab3-2/src -B build/lab3-2
cmake --build build/lab3-2
./build/lab3-2/lab3-2 lab3-2/examples/example.txt
```

## Prerequisites

All C++ labs require CMake 3.22+ and a C++20 compiler. Additional dependencies
are local to the labs that use them:

- Boost.Regex: `lab1-2`;
- Flex 2.6+: `lab1-5`, `lab2-3`, `lab2-4`, and both `lab3-1` applications;
- Boost.JSON 1.75+: `lab2-4`;
- Boost.Program_options and Boost headers: `lab3-1/generator`;
- Bison 3.8+: `lab3-2`;
- Go 1.22+: `lab2-1` (the version requirement is recorded in `go.mod`);
- Python 3: `lab2-2` and `lab3-3`.

The full verification was performed on Debian 13 with CMake 3.31.6, GCC 14.2,
Boost 1.83, Flex 2.6.4, Bison 3.8.2, Go 1.24.4, and Python 3.13.5.

## Build and test

The repository script keeps every build tree outside the source directories:

```sh
./scripts/check.sh
```

Set `BUILD_ROOT` to retain the independent CMake build directories, or
`BUILD_JOBS` to choose build parallelism. See the showcase READMEs for focused
commands and direct CTest invocations.

## Reports and provenance

The Russian-language reports capture the original submissions. They include
historical source snapshots, so the current source directories—not embedded
report listings—are authoritative. See [`PROVENANCE.md`](PROVENANCE.md) for the
ownership split and the status of supplied materials.

## Limitations

These are independent educational variants, not one production compiler, and
each C++ assignment intentionally retains its own build root. `lab1-1` depends
on historical P5 tooling and is not part of the default executable check.
Several support files were supplied for coursework; consequently the repository
does not assert a single blanket license.
