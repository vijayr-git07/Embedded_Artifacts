from pathlib import Path

from context_builder.dependency_graph import build_dependency_graph
from context_builder.symbol_ownership_clang import build_symbol_ownership_clang
from context_builder.interface_contracts_clang import build_interface_contracts_clang
from context_builder.call_graph_clang import build_call_graph_clang
from context_builder.shared_state_clang import build_shared_state_clang
from context_builder.peripheral_ownership_clang import build_peripheral_ownership_clang
from context_builder.protected_regions import build_protected_regions
from context_builder.discovery.include_discovery import discover_include_paths
from context_builder.discovery.compiler_flag_discovery import discover_compiler_flags
from utils.file_loader import discover_application_files


def build_context(project_root):
    """Build the consolidated context for one STM32 application project."""
    project_root = str(Path(project_root).resolve())
    if not Path(project_root).is_dir():
        raise NotADirectoryError(f"Project directory does not exist: {project_root}")

    application_files = discover_application_files(project_root)
    if not application_files:
        raise RuntimeError(
            "No application .c/.h files found under Core/Src or Core/Inc: "
            f"{project_root}"
        )

    print(f"Discovered {len(application_files)} application files.", flush=True)

    include_paths = discover_include_paths(project_root)
    build_dir = Path(project_root) / "Debug"
    flags = discover_compiler_flags(str(build_dir)) if build_dir.is_dir() else {
        "include_paths": [],
        "defines": [],
    }

    # Combine project include directories with include paths captured from
    # generated compiler makefiles. Preserve order and remove duplicates.
    include_paths = list(dict.fromkeys(include_paths + flags.get("include_paths", [])))
    defines = flags.get("defines", [])

    print("[1/7] Building dependency graph...", flush=True)
    dependency_graph = build_dependency_graph(application_files)

    print("[2/7] Building symbol ownership...", flush=True)
    symbol_ownership = build_symbol_ownership_clang(
        project_root, include_paths, defines
    )

    print("[3/7] Building interface contracts...", flush=True)
    interface_contracts = build_interface_contracts_clang(
        project_root, include_paths, defines
    )

    print("[4/7] Building call graph...", flush=True)
    call_graph = build_call_graph_clang(project_root, include_paths, defines)

    print("[5/7] Building shared state map...", flush=True)
    shared_state_map = build_shared_state_clang(
        project_root, include_paths, defines
    )

    print("[6/7] Building peripheral ownership...", flush=True)
    peripheral_ownership = build_peripheral_ownership_clang(
        project_root, include_paths, defines
    )

    print("[7/7] Building protected regions...", flush=True)
    protected_regions = build_protected_regions(application_files)

    context = {
        "dependency_graph": dependency_graph,
        "symbol_ownership": symbol_ownership,
        "interface_contracts": interface_contracts,
        "call_graph": call_graph,
        "shared_state_map": shared_state_map,
        "protected_regions": protected_regions,
        "peripheral_ownership": peripheral_ownership,
    }

    print("Project context extraction completed.", flush=True)
    return context
