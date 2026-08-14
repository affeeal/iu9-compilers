# Self-applicable LL(1) parser generator

This lab implements a predictive parser-table generator and uses that generator
to describe its own input grammar. “Self-applicable” means that the checked-in
table used to parse grammar descriptions can be regenerated from
`self-describing_grammar.txt` by the same application.

The pipeline is:

```text
grammar text -> Flex lexer -> predictive parser -> derivation tree -> AST
             -> semantic checks -> FIRST/FOLLOW -> LL(1) table
             -> generated analyzer_table.cc
```

`apps/generator` implements this pipeline. `apps/calculator` is a small consumer
whose checked-in predictive table recognizes arithmetic expressions before the
semantic pass evaluates them.

## Build

Run all commands from the repository root. The applications deliberately have
separate CMake projects:

```sh
cmake -S lab3-1/apps/generator/src -B build/lab3-1-generator
cmake --build build/lab3-1-generator

cmake -S lab3-1/apps/calculator/src -B build/lab3-1-calculator
cmake --build build/lab3-1-calculator
```

Generate a calculator table without modifying checked-in source:

```sh
./build/lab3-1-generator/generator \
  --grammar lab3-1/apps/generator/examples/arithmetic_grammar.txt \
  --template lab3-1/apps/generator/templates/analyzer_table.cc \
  --table build/arithmetic_analyzer_table.cc
```

Regenerate the generator's own table to another temporary output:

```sh
./build/lab3-1-generator/generator \
  --grammar lab3-1/apps/generator/examples/self-describing_grammar.txt \
  --template lab3-1/apps/generator/templates/analyzer_table.cc \
  --table build/self_analyzer_table.cc
```

Run the calculator example:

```sh
./build/lab3-1-calculator/calculator \
  lab3-1/apps/calculator/examples/example.txt
# 15
```

The input is `1 + 2 * (3 + 4)`, so multiplication and parentheses demonstrate
that the generated predictive structure preserves the intended precedence.

Run the regression suites with:

```sh
ctest --test-dir build/lab3-1-generator --output-on-failure
ctest --test-dir build/lab3-1-calculator --output-on-failure
```

## Grammar notation and limitations

Nonterminals match `[A-Za-z][A-Za-z0-9]*`; terminals are nonempty quoted strings
without embedded quotes or newlines. Productions use `->`, alternatives use
`'or`, epsilon is `'epsilon`, and each rule ends with `'end`. Exactly one rule
must be marked `'axiom`. `#` starts a line comment.

The generator accepts LL(1) grammars only. It reports duplicate axioms, duplicate
or unused rules, undefined nonterminals, and table conflicts; it does not remove
left recursion or factor a grammar automatically. Output, template, and grammar
paths are required explicitly so behavior never depends on the working
directory.

The original Russian-language submission is available as the
[`report`](report/report.md).
