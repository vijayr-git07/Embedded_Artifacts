import os
from pathlib import Path

from clang import cindex


class ClangEngine:
    """Create Clang translation units using the runner's installed LibClang."""

    def __init__(self):

        # Configure LibClang only once per Python process.
        if not cindex.Config.loaded:

            # Set LIBCLANG_PATH when LibClang is not discoverable
            # through the system's normal library search paths.
            library_path = os.environ.get("LIBCLANG_PATH")

            if library_path:
                library_path = str(
                    Path(library_path).expanduser().resolve()
                )

                if not Path(library_path).is_file():
                    raise FileNotFoundError(
                        f"LIBCLANG_PATH does not point to a file: {library_path}"
                    )

                cindex.Config.set_library_file(library_path)

        # Create the Clang index using the configured LibClang instance.
        self.index = cindex.Index.create()

    def parse_file(self, file_path, include_paths=None, defines=None):

        file_path = str(Path(file_path).resolve())
        include_paths = include_paths or []
        defines = defines or []

        args = ["-fsyntax-only", "-std=gnu11"]

        suffix = Path(file_path).suffix.lower()

        if suffix in {".c", ".h"}:
            args.extend(["-x", "c"])

        for path in include_paths:
            args.append(f"-I{Path(path).resolve()}")

        for define in defines:
            args.append(f"-D{define}")

        translation_unit = self.index.parse(file_path, args=args)

        for diagnostic in translation_unit.diagnostics:

            if diagnostic.severity >= cindex.Diagnostic.Error:
                print(f"[CLANG ERROR] {diagnostic}", flush=True)

            elif diagnostic.severity == cindex.Diagnostic.Warning:
                print(f"[CLANG WARNING] {diagnostic}", flush=True)

        return translation_unit
