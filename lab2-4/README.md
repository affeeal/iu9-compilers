# Recursive-descent parser with JSON AST output

This lab parses a typed functional-language subset with integer, list, and tuple
types, pattern matching in function clauses, list construction, arithmetic, and
function calls. Flex performs tokenization; a hand-written recursive-descent
parser applies the grammar and emits the resulting AST through Boost.JSON.

## Build and run

From the repository root:

```sh
cmake -S lab2-4/src -B build/lab2-4
cmake --build build/lab2-4
./build/lab2-4/lab2-4 lab2-4/examples/program.txt
ctest --test-dir build/lab2-4 --output-on-failure
```

The output is one JSON document. A shortened excerpt looks like:

```json
{
  "funcs": [
    {
      "ident_code": 0,
      "type": {"input": {"discriminator_type": "tuple_type"}},
      "body": {"sents": []}
    }
  ]
}
```

Identifier names are interned, so AST nodes contain numeric `ident_code` values.
The exact language definitions are under [`specs`](specs/), and the original
Russian-language submission is in the [`report`](report/report.md).
