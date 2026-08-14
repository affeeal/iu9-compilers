#!/usr/bin/env python3

import pathlib
import subprocess
import sys
import tempfile


def run(formatter: pathlib.Path, source: pathlib.Path) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        [formatter, source], text=True, capture_output=True, check=False
    )


def main() -> None:
    formatter = pathlib.Path(sys.argv[1])
    example = pathlib.Path(sys.argv[2])

    first = run(formatter, example)
    assert first.returncode == 0, first.stderr
    assert "zip (*int, *int)" in first.stdout

    with tempfile.TemporaryDirectory() as directory_name:
        directory = pathlib.Path(directory_name)
        formatted = directory / "formatted.txt"
        formatted.write_text(first.stdout, encoding="utf-8")
        second = run(formatter, formatted)
        assert second.returncode == 0, second.stderr
        assert second.stdout == first.stdout

        invalid = directory / "invalid.txt"
        invalid.write_text("not valid source", encoding="utf-8")
        assert run(formatter, invalid).returncode != 0
        assert run(formatter, directory / "missing.txt").returncode != 0


if __name__ == "__main__":
    main()
