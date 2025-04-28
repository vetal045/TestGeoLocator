#!/usr/bin/env python3

import subprocess
import sys
import os
import shutil
import platform

def check_python_version():
    if sys.version_info < (3, 8):
        print("Python 3.8+ or higher is required.")
        sys.exit(1)

def check_tool(name):
    if shutil.which(name) is None:
        print(f"Required tool '{name}' not found in PATH.")
        if name == "ninja":
            print("Install Ninja: pip install ninja")
        elif name in ["gcc", "g++"]:
            print("Install MinGW-w64 (Windows) or Xcode Command Line Tools (macOS)")
        sys.exit(1)

def detect_preset():
    system = platform.system()
    if system == "Windows":
        return "windows-release"
    elif system == "Darwin":
        return "macos-release"
    else:
        print(f"Unsupported OS: {system}")
        sys.exit(1)

def clean_build_folder(build_dir):
    if os.path.exists(build_dir):
        print(f"Removing build directory: {build_dir}")
        shutil.rmtree(build_dir)

def configure_build(preset):
    try:
        subprocess.run(["cmake", "--preset", preset], check=True)
    except subprocess.CalledProcessError:
        print("CMake configuration failed.")
        sys.exit(1)

def build_target():
    try:
        subprocess.run(["cmake", "--build", "build"], check=True)
    except subprocess.CalledProcessError:
        print("CMake build failed.")
        sys.exit(1)

def build_preprocessor():
    try:
        subprocess.run(["cmake", "--build", "build", "--target", "preprocessor"], check=True)
    except subprocess.CalledProcessError:
        print("Preprocessor build failed.")
        sys.exit(1)

def run_tests():
    try:
        subprocess.run(["ctest", "--test-dir", "build", "--output-on-failure"], check=True)
    except subprocess.CalledProcessError:
        print("Tests failed.")
        sys.exit(1)

def generate_docs():
    if shutil.which("doxygen") is None:
        print("Doxygen not found. Install it manually.")
        sys.exit(1)
    docs_path = os.path.join("docs", "html")
    if os.path.exists(docs_path):
        shutil.rmtree(docs_path)
    subprocess.run(["doxygen", "Doxyfile"], check=True)

def main():
    check_python_version()
    check_tool("cmake")
    check_tool("ninja")
    check_tool("gcc")
    check_tool("g++")

    preset = detect_preset()
    print(f"Detected build preset: {preset}")

    args = sys.argv[1:]

    if "--full-rebuild" in args:
        clean_build_folder("build")

    configure_build(preset)

    if "--preprocessor" in args:
        build_preprocessor()
        sys.exit(0)

    if "--docs" in args:
        generate_docs()
        sys.exit(0)

    if "--build-only" in args:
        build_target()
        sys.exit(0)

    if "--test" in args:
        build_target()
        run_tests()
        sys.exit(0)

    print("Usage: build.py [--build-only | --test | --preprocessor | --docs | --full-rebuild]")
    sys.exit(1)

if __name__ == "__main__":
    main()
