from pathlib import Path
import subprocess
import sys


ROOT = Path(__file__).resolve().parent.parent
NANOC = ROOT / "build" / "debug" / "nanoc"
INPUT = ROOT / "tests" / "preprocessor" / "input.c"


def normalized(text: str) -> str:
    return "\n".join(
        line for line in text.splitlines() if not line.startswith("# ")
    ) + "\n"


def main() -> int:
    nanoc = subprocess.run(
        [str(NANOC), "-E", str(INPUT)],
        cwd=ROOT,
        check=True,
        capture_output=True,
        text=True,
    )
    if nanoc.stderr:
        sys.stderr.write(nanoc.stderr)
        return 1

    gcc = subprocess.run(
        ["gcc", "-E", "-P", str(INPUT)],
        cwd=ROOT,
        check=True,
        capture_output=True,
        text=True,
    )
    if normalized(nanoc.stdout) != gcc.stdout:
        sys.stderr.write("preprocessor output differs from gcc\n")
        return 1

    print("preprocessor: ok")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
