# Geolocation Lookup Application

## Project Overview

This project is a high-performance IP geolocation lookup tool written in modern **C++20**, designed for maximum efficiency, minimal memory footprint, and clean architectural organization.

It supports:
- Fast loading of large IP geolocation datasets
- Ultra-fast IP lookups
- Strict and simple CLI request-response protocol (`LOAD`, `LOOKUP <IP>`, `EXIT`)

The application is built to run reliably on both **Windows** (using MinGW-w64 GCC) and **macOS** (using Apple Clang).

Special attention was paid to clean code structure, separation of concerns, and production-grade C++ development practices.

---

## Requirements and Pre-conditions

### Windows
- [CMake](https://cmake.org/download/) >= 3.16
- [Python 3.8+](https://www.python.org/)
- [Ninja](https://ninja-build.org/) (`pip install ninja`)
- [MinGW-w64 GCC](https://www.mingw-w64.org/) >= 13 (`choco install mingw`)
- Doxygen (optional, `choco install doxygen`)

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
OK    71.6.28.0 US San Jose Memory usage: 415.46mb Lookup time: 54μs
...
Final points for 10 measurements: 4404.14
```

### Known Issue (`psutil.NoSuchProcess`):

On both Windows and macOS, the script might end with a `psutil.NoSuchProcess` error. This occurs because the script attempts to measure the application's memory usage after sending an `EXIT` command, by which point the application has already terminated.

**Important**: This error is harmless and does not indicate any issue with the geolocation application itself. You can safely ignore it if preceding performance metrics are displayed correctly.

---

## Optimization Techniques (Detailed)

### 1. Sorted Records + Binary Search
Geolocation IP ranges are loaded sorted by start IP address. At runtime, lookups are performed using `std::lower_bound`, providing logarithmic O(log N) time complexity. This guarantees fast lookups even with millions of IP ranges.

### 2. Chunked Data Partitioning
Records are partitioned into "chunks" based on IP address ranges. Lookup first selects a chunk based on IP, drastically reducing the number of records to scan during binary search. This further optimizes average-case lookup time.

### 3. Efficient IP Parsing
A fast manual IP string to integer parser is used instead of costly `std::istringstream` or regex-based parsing, improving lookup performance significantly.

### 4. Preprocessing CSV into Binary `.geo` Format
Instead of parsing CSV on every `LOAD`, the `.geo` format stores pre-serialized data. This reduces I/O operations, parsing overhead, and accelerates database loading by an order of magnitude.

### 5. Minimal Dynamic Memory Usage
After initial database load, the application uses preallocated vectors without further dynamic memory allocations, leading to predictable and minimal memory footprint during lookups.

---

## Challenges and Solutions (Expanded)

### 1. Slow CSV Parsing at Runtime
**Challenge:** Parsing and loading large CSV datasets during application startup caused unacceptable delays.

**Solution:** Designed a custom binary `.geo` format and implemented a separate preprocessing tool. At runtime, the application simply reads the `.geo` file directly into memory, skipping costly parsing.

### 2. High Memory Consumption
**Challenge:** Loading all CSV records as strings and full objects caused significant memory usage, especially for large datasets.

**Solution:** Introduced minimal, compact in-memory structures (`GeoRecord`) and aggressively used move semantics to reduce copy overhead. Optimized memory layout to align access patterns.

### 3. Windows Filesystem Behavior Causing Test Failures
**Challenge:** On Windows, files cannot be deleted if they are still opened by a stream, causing test failures when cleaning up temporary files.

**Solution:** Explicitly closed file streams (`ifs.close()`) before calling `fs::remove(...)` in tests, ensuring Windows compatibility without affecting Unix systems.

### 4. Environment Inconsistencies
**Challenge:** Different machines might lack tools (e.g., Ninja, GCC) or have misconfigured PATHs.

**Solution:** `build.py` script automatically checks for required tools before proceeding. Clear error messages guide the user on how to fix missing dependencies.

### 5. Dealing with External Python Script Issues
**Challenge:** Python's `psutil` package occasionally throws `NoSuchProcess` during shutdown phase on Windows.

**Solution:** Since it does not affect application correctness, the application exits cleanly according to protocol. Minor psutil issues are documented and safely ignored.

---

## Known Issues and Notes

| Problem | Solution |
|:--------|:---------|
| Ninja not found | Install Ninja (`pip install ninja`) |
| GCC not found (Windows) | Install MinGW-w64 (`choco install mingw`) |
| PATH not updated | Run `refreshenv` or restart the terminal |
| psutil errors during Python test | Harmless; related to external testing script, not application |
| Doxygen missing | Install manually (`brew install doxygen` or `choco install doxygen`) |

---

## License

This project is licensed under the MIT License.  
It incorporates [fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser) under the BSD License.

---

## Author

Developed by **Vitalii Ryzhov**  
Focus: high-performance C++20 code, clean production-ready architecture, cross-platform reliability.

---

