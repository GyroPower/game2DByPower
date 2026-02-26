import shutil
import subprocess
from pathlib import Path


def build_ninja_debug():
    build_dir = Path(__file__).resolve().parent / "build" / "Debug"
    ninja_command = ["ninja", "-C", str(build_dir)]

    result = subprocess.run(ninja_command, capture_output=False, text=True)

    if result.returncode == 0:
        print("-- ninja success")

    else:
        print("-- ninja failed")


build_ninja_debug()
