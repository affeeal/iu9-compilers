#!/usr/bin/env python3

import pathlib
import subprocess
import sys
import tempfile


def main() -> None:
    calculator = pathlib.Path(sys.argv[1])
    example = pathlib.Path(sys.argv[2])

    result = subprocess.run(
        [calculator, example], text=True, capture_output=True, check=False
    )
    assert result.returncode == 0, result.stderr
    assert result.stdout.strip() == "15", result.stdout

    with tempfile.TemporaryDirectory() as directory:
        missing = pathlib.Path(directory) / "missing.txt"
        result = subprocess.run(
            [calculator, missing], text=True, capture_output=True, check=False
        )
        assert result.returncode != 0


if __name__ == "__main__":
    main()
