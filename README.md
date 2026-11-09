# chronovex

Chronovex is a standalone C99 timing library for sequence periods, jitter, deadlines, wrap-safe windows and deterministic scheduling helpers.

## Scope

The package contains 31 public headers and 4 implementation units. It is portable C99, uses no mandatory heap or operating-system service, and keeps caller-owned state explicit where applicable.

## Build and test

```sh
cmake -S . -B build -DCHRONOVEX_BUILD_TESTS=ON
cmake --build build --config Release
ctest --test-dir build --output-on-failure -C Release
```

## Validation status

Host validation is run during extraction. ESP32-S3 validation and isolated footprint measurements are pending unless explicitly recorded; no hardware result is claimed.

## Architecture and integration

The library contains independent, deterministic C99 timing operations with
caller-owned state where state is required. Add `include/` and link `chronovex`,
or compile selected `src/*.c` files directly.

## API, configuration and dependencies

Public headers are under `include/chronovex/timing`. `CHRONOVEX_BUILD_TESTS`
controls host tests. There are no mandatory third-party, OS, heap or thread
dependencies; wrap and deadline contracts are documented per header.

## Resource, performance and limitations

The enabled host suite passes 2/2 tests. Embedded flash, RAM, stack, cycle and
worst-case timing measurements are `PENDING`. HW NOT VERIFIED; do not infer MCU
timing from the host build.

## Version, security, license and origin

Versioning is independent with semantic-versioning intent before 1.0. Apache
License 2.0, copyright Vanderhell. The package is self-contained and does not
provide security or synchronization by itself.

## Name and origin

Chronovex combines chronos (time) with vector-like execution, reflecting bounded time and schedule calculations.
Its technical scope is bounded timing and scheduling calculations.

## License

Apache License 2.0, copyright Vanderhell. See LICENSE and NOTICE.
