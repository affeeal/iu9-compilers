#!/usr/bin/env python3

import json
import pathlib
import subprocess
import sys
import tempfile


def run(parser: pathlib.Path, source: pathlib.Path) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        [parser, source], text=True, capture_output=True, check=False
    )


def main() -> None:
    parser = pathlib.Path(sys.argv[1])
    example = pathlib.Path(sys.argv[2])

    result = run(parser, example)
    assert result.returncode == 0, result.stderr
    document = json.loads(result.stdout)
    assert isinstance(document.get("funcs"), list)
    assert document["funcs"], document

    with tempfile.TemporaryDirectory() as directory_name:
        directory = pathlib.Path(directory_name)
        invalid = directory / "invalid.txt"
        invalid.write_text("not valid source", encoding="utf-8")
        assert run(parser, invalid).returncode != 0
        assert run(parser, directory / "missing.txt").returncode != 0


if __name__ == "__main__":
    main()
