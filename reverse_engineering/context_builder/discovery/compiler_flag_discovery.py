import re
from pathlib import Path


def discover_compiler_flags(debug_dir):

    include_paths = set()
    defines = set()

    for mk_file in Path(debug_dir).rglob("*.mk"):

        try:
            content = mk_file.read_text(
                errors="ignore"
            )

            include_paths.update(
                re.findall(
                    r"-I([^\s]+)",
                    content
                )
            )

            defines.update(
                re.findall(
                    r"-D([^\s]+)",
                    content
                )
            )

        except Exception:
            pass

    return {
        "include_paths": sorted(include_paths),
        "defines": sorted(defines)
    }
