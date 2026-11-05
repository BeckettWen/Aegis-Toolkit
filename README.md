# Aegis Toolkit — X-20

Aegis Toolkit is a collection of C++ utilities used by the **Heartstrings Engine Series**. The current codebase includes an experimental memory manager, a state machine, and a lightweight thread-pool interface.

> **Status:** This project is under active development. Some APIs are prototypes and may be incomplete or change without notice. In particular, do not rely on the memory manager's “defender” API as a production security boundary.

## Components

| Header | Namespace | Description |
| --- | --- | --- |
| `Aegis.hpp` | `Aegis_MemoryManager` | Chunk-based memory allocation, data writing/reading, and experimental memory compression and token-checking APIs. |
| `StateMachine.hpp` | `Aegis_stateMachine` | A templated state machine with registered transitions and optional actions. |
| `ThreadPool.hpp` | `Aegis_ThreadPool` | A `std::jthread`-based task runner with basic shutdown and task-joining operations. |
| `VersionInfo.h` | — | Shared version metadata; currently identifies the release as **Selenium 10286**. |

The headers are intended to be included directly in a C++ project. For example:

```cpp
#include "Aegis.hpp"
#include "StateMachine.hpp"
#include "ThreadPool.hpp"
```

The memory manager currently allocates in 1 MiB chunks and returns allocation indices (handles). The state machine is parameterized by the application's state and event types; transitions are registered with an action and a destination state.

## Requirements

- A C++23-compatible compiler and standard library (`std::expected` is used).
- For the included Xcode project: Xcode with the macOS SDK.
- GoogleTest for the test target and Google Benchmark for the benchmark target.

The Xcode project currently contains machine-specific Homebrew search paths for GoogleTest and Google Benchmark. If those dependencies are installed elsewhere, update the header and library search paths in the project settings.

## Build and test with Xcode

1. Open `X-20_xcode.xcodeproj` in Xcode.
2. Select the `test` or `benchmark` scheme/target and build it.
3. Run the `test` target to execute the GoogleTest suite.

The test sources are in `tests/`. They cover memory-manager behavior and state-machine transitions. The benchmark target contains memory-manager benchmarks.

## Repository layout

```text
.
├── Aegis.hpp
├── StateMachine.hpp
├── ThreadPool.hpp
├── VersionInfo.h
├── X-20_xcode.xcodeproj/
└── tests/
    ├── benchmark_memoryManager.cpp
    ├── test_main.cpp
    ├── test_memoryManager.cpp
    └── test_statemachine.cpp
```

## Notes

- The API and implementation are evolving; review the headers and tests before integrating them into another project.
- The test executable currently applies a GoogleTest filter in `tests/test_main.cpp`, so it may run only a subset of the available tests by default.
- This repository does not currently provide a standalone package manager manifest or cross-platform build script.

## Project

See the [Aegis-Toolkit repository](https://github.com/BeckettWen/Aegis-Toolkit) for the source and project history.
