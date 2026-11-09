# Chronovex
![CI](https://github.com/Vanderhell/chronovex/actions/workflows/ci.yml/badge.svg) ![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg) ![Language](https://img.shields.io/badge/language-C99-informational.svg)

![Chronovex banner](docs/banner.svg)


Wrap-safe deadlines, periods, durations, timeouts and schedule calculations.

## Overview
**Timing and scheduling.** Wrap-safe deadlines, periods, durations, timeouts and schedule calculations.

## Engineering problem
Bare-metal systems need deterministic time arithmetic that remains correct across counter wrap.

## Features
- Portable C99-oriented implementation.
- Deterministic integer or fixed-point behavior documented at function level.
- No mandatory heap, threads or operating-system services.
- Separate translation units for selective linking.

## Architecture
The public surface consists of 4 headers and 4 callable API entries under include/. Implementations are in src/ and tests are in tests/. State, buffers and output ownership are explicit in each header contract.

## Public API
The complete symbol-by-symbol reference is in docs/API_REFERENCE.md. It lists every public header, callable symbol, include path and available contract comment.

## Integration
```sh
cmake -S . -B build -DCHRONOVEX_BUILD_TESTS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

For minimal firmware, include only the required header and compile the corresponding source translation unit. No collection-wide dependency is required.

## Configuration and dependencies
The CMake test option is CHRONOVEX_BUILD_TESTS. The library has no mandatory third-party dependency. Platform-specific link requirements are documented in CMake and the affected API header.

## Numeric and safety contracts
Read the individual public header before use. It defines widths, representation, valid ranges, rounding, overflow/saturation/wrap behavior, invalid-input handling and whether output is left unchanged on failure. APIs are stateless unless a caller-owned state object is explicitly present.

## Testing and validation
2/2 enabled host tests passed.
See docs/TESTING.md for reproducible commands, coverage boundaries and hardware status.

## Resource status
Embedded flash, RAM, stack and cycle measurements are reported only when measured for this product and configuration. Unmeasured values are marked PENDING; host measurements are not MCU measurements.

## Hardware status
Hardware validation is reported in docs/TESTING.md. HW NOT VERIFIED is used where no reproducible target run is available.

## Limitations
Preserved tests outside this product boundary are not silently treated as product coverage. Unsupported ranges and invalid inputs are defined by the public headers.

## Versioning and license
Version 0.1.0. Public API compatibility is maintained within the 0.x series as documented changes are introduced. Apache License 2.0, copyright Vanderhell; see LICENSE and NOTICE.

## Origin
This repository is a self-contained product library. Historical provenance is maintained in the central catalog, while this repository documents the usable public API and its behavior.
## Family guide and examples

Read [docs/FAMILY_GUIDE.md](docs/FAMILY_GUIDE.md) for family-level selection guidance, representations, integration checklist and a complete primitive map. Read [docs/API_REFERENCE.md](docs/API_REFERENCE.md) for each header's contract and a test-derived call example. The examples intentionally preserve the validated API units and do not hide failure handling.
