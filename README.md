# Geolocation Lookup Application

## Project Overview

This project is a high-performance IP geolocation lookup tool written in modern **C++20**, designed for maximum efficiency, minimal memory footprint, and clean architectural organization.

It supports:
- Fast loading of large IP geolocation datasets
- Fast IP lookups
- Strict and simple CLI request-response protocol (`LOAD`, `LOOKUP <IP>`, `EXIT`)

The application is built to run reliably on both **Windows** (using MinGW-w64 GCC) and **macOS** (using Apple Clang).

Special attention was paid to clean code structure, separation of concerns, and production-grade C++ development practices.

---

## Requirements and Pre-conditions

### Windows
- [CMake](https://cmake.org/download/) >= 3.16
- [Python 3.8+](https://www.python.org/)
- [Ninja](https://ninja-build.org/) (`pip install ninja`)
- [MinGW-w64 GCC](https://www.mingw-w64.org/) >= 13 (`choco install mingw -y`)
- Doxygen (optional, `choco install doxygen.install -y`)

### macOS
- Xcode Command Line Tools (`xcode-select --install`)
- Homebrew packages:
  ```bash
  brew install cmake ninja coreutils doxygen
  ```

**System Preconditions:**
- Ensure `gcc` or `clang` compiler supports C++20.
- Ensure `ninja` is installed and available in `PATH`.
- On Windows, use `refreshenv` after installation via Chocolatey.
- Python 3.8+ must be available for running helper scripts.

---

## Project Structure and Component Overview

| Path | Responsibility |
|:-----|:---------------|
| `src/locator/` | Core geolocation logic: efficient IP lookup using sorted chunks (`GeoLocator`) |
| `src/loaders/` | Loaders for binary `.geo` database files with minimal memory overhead |
| `src/command/` | Parsing CLI commands and handling command execution (`CommandParser`, handlers) |
| `src/service/` | Central dispatching of parsed commands to handlers (`CommandService`) |
| `preprocessor/` | CSV-to-GEO preprocessing tool to accelerate loading |
| `include/` | Common interfaces (`IGeoRecordProvider`, `IGeoDatabaseLoader`) and data types |
| `tests/` | Unit and integration tests, using GoogleTest framework |
| `docs/` | Optional Doxygen-generated documentation |
| `data/` | Geolocation datasets: `database.csv`, generated `database.geo` |

Each module is isolated with clear single-responsibility principles and minimal coupling.

---

## Building the Project

Use the unified `build.py` script for all operations.

Typical full clean rebuild:
```bash
python build.py --full-rebuild --build-only
```

Other useful commands:

| Purpose | Command |
|:--------|:--------|
| Build without cleaning | `python build.py --build-only` |
| Build and run unit tests | `python build.py --test` |
| Build only preprocessor tool | `python build.py --preprocessor` |
| Generate Doxygen documentation | `python build.py --docs` |

**Note:**
- `build.py` automatically detects OS and chooses `windows-release` or `macos-release` CMake preset.
- Ninja is used for high-speed builds.

---

## Database Preprocessing

Since loading raw `.csv` at runtime is slow, the project provides a preprocessing tool that converts `.csv` into optimized `.geo` binary format.

Usage:
```bash
./build/preprocessor/preprocessor.exe ./data/database.csv ./data/database.geo
```
(on macOS, no `.exe` extension)

**Advantages of `.geo` format:**
- Minimizes load time drastically
- Reduces runtime parsing costs
- Enables efficient memory access patterns

It is strongly recommended to always use `.geo` files during application usage.

---

## Application Usage Protocol

The application communicates via standard input/output.

**Supported Commands:**
| Command | Response |
|:--------|:---------|
| `LOAD` | `OK\n` (or `ERR\n` on failure) |
| `LOOKUP <IPv4>` | `<CountryCode>,<City>\n` (or `ERR\n` if not found or invalid input) |
| `EXIT` | `OK\n` before graceful shutdown |

**Example Session:**
```
> LOAD
< OK
> LOOKUP 1.0.0.0
< US,Los Angeles
> EXIT
< OK
```

---

## Python Test Script Usage

The provided `geolocation_test.py` script validates protocol compatibility and measures performance metrics (database load time, memory usage, IP lookup latency).

### Example Command:

```bash
python geolocation_test.py --executable ./build/geolocator/geolocator --database ./data/database.geo
```

### Sample Output:

```
Database loaded Memory usage: 415.46mb Load time: 223ms
OK    1.0.0.0 US Los Angeles Memory usage: 415.46mb Lookup time: 46μs
...
Final points for 10 measurements: 4404.14
```

### Known Issue (`psutil.NoSuchProcess`):

The script might end with a `psutil.NoSuchProcess` error. This occurs because the script attempts to measure the application's memory usage after sending an `EXIT` command, by which point the application has already terminated.

**Important**: This error is harmless and does not indicate any issue with the geolocation application itself. You can safely ignore it if preceding performance metrics are displayed correctly.

---

## Benchmark Results and Platform Differences

### Windows (Dell G15 5530, i5-13450HX, 32 GB RAM)
```
Database loaded Memory usage: 415.52mb Load time: 240ms
Final points for 10 measurements: 4443.11
```

### macOS (MacBook Pro 14" M1 Pro, 16 GB RAM)
```
Database loaded Memory usage: 543.38mb Load time: 788ms
Final points for 10 measurements: 6265.17
```

### Observations and Differences
- **Database loading**: Faster on Windows due to `CreateFileMapping` and `MapViewOfFile`, optimized for NTFS.
- **Memory usage**: Lower on Windows. macOS incurs additional overhead for memory mapping (`mmap`).
- **Lookup speed**: Very close; macOS sometimes gives slightly faster microsecond lookup times due to M1 architecture optimizations.
- **Load method**: Platform-specific mapping used in `BinaryGeoDatabaseLoader`, with Windows using WinAPI and macOS using POSIX `mmap`.

---

## Optimization Techniques (Detailed)

### 1. Platform-Specific Database Loading
- Windows: `CreateFileMapping` + `MapViewOfFile`
- macOS: `mmap`
- Both platforms benefit from zero-copy loading.

### 2. Sorted Records + Binary Search
Efficient `std::lower_bound` on sorted IP ranges ensures O(log N) lookup time.

### 3. Chunked Data Partitioning
Chunking IP space reduces search scope drastically, improving average-case lookup time.

### 4. Efficient IP Parsing
Custom fast manual IP parsing instead of slow regex or stringstreams.

### 5. Preprocessing CSV to `.geo`
Preprocessing reduces load time from seconds to milliseconds by avoiding CSV parsing at runtime.

### 6. Memory-Efficient In-Memory Structures
Careful structure layout and move semantics reduce memory footprint.

---

## Challenges and Solutions

| Challenge | Solution |
|:----------|:---------|
| Slow CSV parsing at startup | Custom `.geo` binary format |
| High memory usage | Compact `GeoRecord` structures and efficient loading |
| Different filesystem behaviors | Platform-specific loading (WinAPI vs POSIX mmap) |
| psutil errors in testing | Ignored; no impact on app correctness |
| Missing build tools | Added auto-checks and clear errors in `build.py` |

---

## Known Issues and Notes

| Problem | Solution |
|:--------|:---------|
| Ninja not found | Install Ninja (`pip install ninja`) |
| GCC not found (Windows) | Install MinGW-w64 (`choco install mingw`) |
| PATH not updated | Run `refreshenv` or restart the terminal |
| psutil errors during Python test | Harmless; does not affect correctness |
| Doxygen missing | Install (`brew install doxygen` / `choco install doxygen`) |

---

## License

This project is licensed under the MIT License.
It incorporates [fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser) under the BSD License.

---

## Author

Developed by **Vitalii Ryzhov**
Focus: high-performance C++20 code, clean production-grade architecture, cross-platform reliability.