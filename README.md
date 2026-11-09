# chronovex

Chronovex is a standalone C99 timing library for sequence periods, jitter, deadlines, wrap-safe windows and deterministic scheduling helpers.

## Scope

This first extraction contains 31 family headers, 4 implementation units and 9 migrated test sources. It is portable C99, uses no mandatory heap or operating-system service, and keeps caller-owned state explicit where applicable.

## Build and test

```sh
cmake -S . -B build -DCHRONOVEX_BUILD_TESTS=ON
cmake --build build --config Release
ctest --test-dir build --output-on-failure -C Release
```

## Validation status

Host validation is run during extraction. ESP32-S3 validation and isolated footprint measurements are pending unless explicitly recorded; no hardware result is claimed.

## Name and origin

Chronovex combines chronos (time) with vector-like execution, reflecting bounded time and schedule calculations.
It originates from PrimitiveCore's timing/time_and_scheduling family.

## License

Apache License 2.0, copyright Vanderhell. See LICENSE and NOTICE.
