#!/usr/bin/env python3

import pathlib
import re
import subprocess
import sys
import tempfile


SYMBOL = re.compile(
    r'\{\s*"([^"\\]*(?:\\.[^"\\]*)*)"\s*,\s*'
    r"Symbol::Type::(k[A-Za-z]+)\s*\}"
)


def run(*args: object) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        [str(arg) for arg in args], text=True, capture_output=True, check=False
    )


def require_failure(
    generator: pathlib.Path,
    grammar: pathlib.Path,
    template: pathlib.Path,
    output: pathlib.Path,
    diagnostic: str,
) -> None:
    result = run(
        generator,
        "--grammar",
        grammar,
        "--template",
        template,
        "--table",
        output,
    )
    assert result.returncode != 0, result.stdout
    assert diagnostic.lower() in result.stderr.lower(), result.stderr


def table_signature(source: pathlib.Path) -> tuple[tuple[str, str], frozenset]:
    text = source.read_text(encoding="utf-8")
    axiom_marker = text.index("axiom_(")
    axiom_match = SYMBOL.search(text, axiom_marker)
    assert axiom_match is not None, f"axiom not found in {source}"

    table_marker = text.index("table_(", axiom_match.end())
    start = text.index("{", table_marker)
    depth = 0
    in_string = False
    escaped = False
    end = None
    for index in range(start, len(text)):
        char = text[index]
        if in_string:
            if escaped:
                escaped = False
            elif char == "\\":
                escaped = True
            elif char == '"':
                in_string = False
            continue
        if char == '"':
            in_string = True
        elif char == "{":
            depth += 1
        elif char == "}":
            depth -= 1
            if depth == 0:
                end = index
                break
    assert end is not None, f"table initializer not closed in {source}"

    records = []
    record_start = start + 1
    depth = 1
    for index in range(start + 1, end + 1):
        char = text[index]
        if char == "{":
            depth += 1
        elif char == "}":
            depth -= 1
        elif char == "," and depth == 1:
            records.append(text[record_start:index])
            record_start = index + 1
    records.append(text[record_start:end])

    signature = []
    for record in records:
        symbols = tuple(SYMBOL.findall(record))
        if symbols:
            assert len(symbols) >= 2, record
            signature.append(symbols)
    return axiom_match.groups(), frozenset(signature)


def main() -> None:
    generator = pathlib.Path(sys.argv[1])
    template = pathlib.Path(sys.argv[2])
    arithmetic = pathlib.Path(sys.argv[3])
    self_grammar = pathlib.Path(sys.argv[4])
    checked_table = pathlib.Path(sys.argv[5])
    calculator_include = pathlib.Path(sys.argv[6])
    generator_include = pathlib.Path(sys.argv[7])
    compiler = pathlib.Path(sys.argv[8])

    help_result = run(generator, "--help")
    assert help_result.returncode == 0, help_result.stderr

    with tempfile.TemporaryDirectory() as directory_name:
        directory = pathlib.Path(directory_name)
        arithmetic_table = directory / "arithmetic.cc"
        repeat_table = directory / "arithmetic-repeat.cc"
        self_table = directory / "self.cc"

        for grammar, output in (
            (arithmetic, arithmetic_table),
            (arithmetic, repeat_table),
            (self_grammar, self_table),
        ):
            result = run(
                generator,
                "--grammar",
                grammar,
                "--template",
                template,
                "--table",
                output,
            )
            assert result.returncode == 0, result.stderr

        assert arithmetic_table.read_bytes() == repeat_table.read_bytes()
        assert table_signature(self_table) == table_signature(checked_table)

        for source, include in (
            (arithmetic_table, calculator_include),
            (self_table, generator_include),
        ):
            result = run(
                compiler,
                "-std=c++20",
                "-fsyntax-only",
                f"-I{include}",
                source,
            )
            assert result.returncode == 0, result.stderr

        invalid_grammars = {
            "undefined": ("'axiom S -> A 'end\n", "undefined nonterminal"),
            "duplicate-axiom": (
                "'axiom S -> \"x\" 'end\n'axiom A -> \"y\" 'end\n",
                "axiom redefinition",
            ),
            "missing-axiom": ("S -> \"x\" 'end\n", "axiom is not defined"),
            "conflict": (
                "'axiom S -> \"x\" 'or \"x\" \"y\" 'end\n",
                "LL(1) conflict",
            ),
        }
        for name, (contents, diagnostic) in invalid_grammars.items():
            grammar = directory / f"{name}.txt"
            grammar.write_text(contents, encoding="utf-8")
            require_failure(
                generator, grammar, template, directory / f"{name}.cc", diagnostic
            )

        require_failure(
            generator,
            directory / "missing-grammar.txt",
            template,
            directory / "missing-grammar.cc",
            "Failed to open file",
        )
        require_failure(
            generator,
            arithmetic,
            directory / "missing-template.cc",
            directory / "missing-template-output.cc",
            "Failed to open file",
        )
        require_failure(
            generator,
            arithmetic,
            template,
            directory / "missing-directory" / "output.cc",
            "Failed to create file",
        )


if __name__ == "__main__":
    main()
