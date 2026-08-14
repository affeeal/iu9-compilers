# Functional-language source formatter

This lab formats a small typed functional language with integer, list, and tuple
types; pattern matching; list construction; arithmetic expressions; function
calls; and `case` expressions.

Its architecture is `Flex scanner -> Bison parser -> AST and identifier table ->
Visitor formatter`. The formatter rebuilds normalized source text from the AST;
comments are intentionally not preserved.

## Build and run

From the repository root:

```sh
cmake -S lab3-2/src -B build/lab3-2
cmake --build build/lab3-2
./build/lab3-2/lab3-2 lab3-2/examples/example.txt
ctest --test-dir build/lab3-2 --output-on-failure
```

For example, this compact input:

```text
polynom1111 int :: int is x = polynom (x, {1, 1, 1, 1}) end
```

is rendered as:

```text
polynom1111 int :: int is
  x = polynom (x, {1, 1, 1, 1})
end
```

The complete source-language grammar is in [`specs/grammar.txt`](specs/grammar.txt),
and the original Russian-language submission is in the
[`report`](report/report.md).

## Limitations

This is a syntax-aware educational formatter, not a configurable style engine.
It emits one built-in layout, discards comments, and assumes syntactically valid
input. Invalid syntax and missing files produce a nonzero exit status.
