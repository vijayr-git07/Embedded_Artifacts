import os


EXCLUDED_SRC_FILES = {
    "stm32f4xx_hal_msp.c",
    "stm32f4xx_it.c",
    "syscalls.c",
    "sysmem.c",
    "system_stm32f4xx.c",
}

EXCLUDED_INC_FILES = {
    "stm32f4xx_hal_conf.h",
    "stm32f4xx_it.h",
}


def discover_application_files(project_root):
    """Return application C and header files from Core/Src and Core/Inc."""
    application_files = []

    core_src = os.path.join(project_root, "Core", "Src")
    core_inc = os.path.join(project_root, "Core", "Inc")

    if os.path.isdir(core_src):
        for filename in sorted(os.listdir(core_src)):
            if filename.endswith(".c") and filename not in EXCLUDED_SRC_FILES:
                application_files.append(os.path.join(core_src, filename))

    if os.path.isdir(core_inc):
        for filename in sorted(os.listdir(core_inc)):
            if filename.endswith(".h") and filename not in EXCLUDED_INC_FILES:
                application_files.append(os.path.join(core_inc, filename))

    return sorted(application_files)
