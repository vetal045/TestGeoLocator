# Project Overview

This project is a high-performance IP geolocation lookup application. It is developed in modern C++17/20 and focuses on loading large datasets quickly, performing efficient IP lookups, and minimizing memory usage. The design prioritizes simplicity, speed, and clean architecture, targeting macOS ARM64 (Apple Silicon).

The application implements a specific request-response CLI protocol with commands: LOAD, LOOKUP <IP>, and EXIT.

---

# Requirements

- macOS (Apple Silicon M1/M2)
- CMake >= 3.16
- Make
- GCC >= 10 or Apple Clang with C++17/20 support
- Python 3.8+ (for optional testing script)
- Coreutils (`realpath`) - install via `brew install coreutils`
- Doxygen (optional, for documentation generation)

---

# Project Structure

| Path | Purpose |
|:-----|:--------|
| `src/locator/` | Core IP lookup logic (`GeoLocator`, `VectorGeoRecordProvider`) |
| `src/loaders/` | Loading binary `.geo` files efficiently (`BinaryGeoDatabaseLoader`) |
| `src/command/` | Protocol command handling (LOAD, LOOKUP, EXIT) |
| `src/service/` | `CommandService` to dispatch commands |
| `preprocessor/` | Preprocessing CSV into efficient `.geo` format |
| `include/` | Common structures and interface definitions |
| `data/` | Geolocation datasets: `database.csv` and `database.geo` |
| `tests/` | Unit tests based on GoogleTest |
| `docs/` | Documentation (optional, via Doxygen) |

---

# Building and Setup

To build everything:

```bash
./make.sh --full-rebuild
```

To build only (without running tests):

```bash
./make.sh --build-only
```

To run unit tests:

```bash
./make.sh --test
```

All build scripts are prepared for macOS environment.

---

# Preprocessing the Database

Loading the raw `.csv` file is slow. To optimize, use the preprocessor to create a compact `.geo` file:

```bash
./preprocess.sh ./data/database.csv ./data/database.geo
```

**Why `.geo`?**
- Binary format minimizes load time
- Enables memory-mapped reading
- Reduces parsing overhead

Use `.geo` file whenever possible for best performance.

---

# How to Use the Application

## Manual Usage (Standard Input/Output)

The application implements a line-based CLI protocol:

```text
> LOAD
< OK
> LOOKUP 71.6.28.0
< US,San Jose
> EXIT
< OK
```

Use command line with redirection or scripting tools to communicate via stdin/stdout.

---

## Using the Provided Python Test Script

There is a provided `geolocation_test.py` script to validate basic protocol compatibility and measure performance.

Example:

```bash
python3 geolocation_test.py --executable ./build/geolocator/geolocator --database ./data/database.geo
```

**Important:**
- The Python script is third-party and cannot be modified.
- On macOS, the script may crash with `psutil.NoSuchProcess` due to differences in process management. This issue is unrelated to the application and comes from the testing script.

---

# Optimization Techniques

## Sorted Records with Binary Search
The IP ranges are stored sorted by start IP. Lookup uses `std::lower_bound`, achieving O(log N) search time.

## Memory Mapping
The binary `.geo` file is memory-mapped on load (mmap). This eliminates full file reads and reduces memory footprint.

## Fast IP String Parsing
Instead of using `std::istringstream`, a manual parser is used to quickly convert IP address strings to integers.

## Minimal Dynamic Allocation
Once loaded, the application uses preallocated structures. No frequent heap allocations during lookup.

---

# Challenges and Solutions

## Slow CSV Parsing
**Problem:** Loading the `.csv` file line by line caused significant startup delays.

**Solution:** A custom binary `.geo` format and a preprocessing tool were created. This reduced load time by approximately 4x.

## Memory Usage with Large Datasets
**Problem:** Reading all data into memory was expensive.

**Solution:** The `.geo` format allows memory mapping without full loading, saving memory and speeding up access.

## Process Management Issues with psutil
**Problem:** During Python testing, especially on macOS, `psutil` raised `NoSuchProcess` errors.

**Solution:** The application was designed to exit cleanly according to the protocol. Python script issues are outside project scope.

## Lookup Time
**Problem:** Naive search algorithms were too slow on large datasets.

**Solution:** Using sorted IP records and binary search (`std::lower_bound`) drastically reduced lookup times.

---

# Potential Improvements

- Implementing Radix Tree or Compressed Trie for even faster lookups.
- Parallel database loading to speed up initialization.
- SIMD optimizations for IP parsing.
- Supporting dynamic reload of databases without downtime.
- Extending support for IPv6 address space.

These were considered but were out of scope due to time constraints.

---

# External Libraries

- [fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser) (BSD License)
  - Used in the preprocessor for efficient CSV parsing.

---

# FAQ / Known Issues

| Problem | Solution |
|:--------|:---------|
| `realpath` not found on macOS | Install coreutils: `brew install coreutils` |
| Python `psutil.NoSuchProcess` | Known issue in testing script, not related to the application |
| Slow load when using `.csv` | Use the provided preprocessor to create `.geo` files |

---

# License

This project is licensed under the MIT License.
It incorporates fast-cpp-csv-parser, licensed under the BSD License.

---

# Author

Developed by Vitalii Ryzhov for a technical assessment challenge, with a focus on performance, clean architecture, and production-grade C++ development.

---

