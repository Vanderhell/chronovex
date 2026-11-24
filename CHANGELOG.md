# Changelog

## 0.2.0 - unreleased

- Added primary `chronovex_`-namespaced functions and header basenames without the legacy prefix.
- Retained v0.1 source-level compatibility headers and macro aliases. Consumers must rebuild because the new archive exports namespaced linker symbols rather than the old `pc_` symbols.
- Added the `Chronovex::Chronovex` CMake target, install/export package configuration, and C++ header smoke coverage.
- Tests and examples are opt-in and disabled by default for dependency consumers.

## 0.1.0

Initial standalone release preparation.
