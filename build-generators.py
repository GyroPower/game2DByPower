import subprocess
import shutil
from pathlib import Path


# Build to use with ninja and clang
def run_process():
    command_conan = [
        "conan",
        "install",
        ".",
        "--build=missing",
        "-pr=clang_debug",
    ]

    result = subprocess.run(command_conan, capture_output=False, text=True)

    if result.returncode == 0:
        print("--conan config done")
    else:
        print("--conan failed")
        return

    script_dir = Path(__file__).resolve().parent

    # if not Path(script_dir / "extern/imgui/ImGuiFileDialog").exists():
    #     command_clone_imgui_file_dialog = [
    #         "git",
    #         "clone",
    #         "https://github.com/aiekick/ImGuiFileDialog.git",
    #         f"{script_dir}/extern/imgui/ImGuiFileDialog",
    #     ]
    #
    #     result = subprocess.run(command_clone_imgui_file_dialog, capture_output=False, text=True)
    #
    #     if result.returncode == 0:
    #         print("ImGuiFileDialog cloned done")
    #     else:
    #         print("ImGuiFileDialog cloned failed")
    #         return
    #
    build_dir = script_dir / "build" / "Debug"

    command = [
        "cmake",
        "-G",
        "Ninja",
        "-B",
        str(build_dir),
        "-DCMAKE_BUILD_TYPE=Debug",
        "-DCMAKE_C_COMPILER=clang",
        "-DCMAKE_CXX_COMPILER=clang++",
        "-DCMAKE_TOOLCHAIN_FILE=build/Debug/generators/conan_toolchain.cmake",
    ]

    result = subprocess.run(command, capture_output=False, text=True)

    if result.returncode == 0:
        print("--cmake config done")
    else:
        print("--cmake failed")
        return

    # Copy compile_commands.json to root for clangd
    source = Path(build_dir / "compile_commands.json")

    if source.exists():
        destination = script_dir / source.name
        _ = shutil.copy2(source, destination)
        source.unlink(missing_ok=True)


run_process()
