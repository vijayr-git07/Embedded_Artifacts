import argparse
import json
import os
from pathlib import Path

from context_builder.context_builder import build_context


def main():
    parser = argparse.ArgumentParser(
        description="Generate consolidated context for an STM32 application project."
    )
    parser.add_argument(
        "--project-path",
        default=os.environ.get("PROJECT_PATH", "../HVAC_STM32_Project"),
        help="Path to the STM32 project directory.",
    )
    parser.add_argument(
        "--output",
        default=os.environ.get("CONTEXT_OUTPUT", "../project_context/context.json"),
        help="Path for the generated consolidated context JSON.",
    )
    args = parser.parse_args()

    project_path = Path(args.project_path).resolve()
    output_path = Path(args.output).resolve()

    context = build_context(str(project_path))
    output_path.parent.mkdir(parents=True, exist_ok=True)

    temporary_path = output_path.with_suffix(output_path.suffix + ".tmp")
    with temporary_path.open("w", encoding="utf-8") as output_file:
        json.dump(context, output_file, indent=4, ensure_ascii=False)
        output_file.write("\n")
    temporary_path.replace(output_path)

    print(f"Context generated successfully: {output_path}", flush=True)


if __name__ == "__main__":
    main()
