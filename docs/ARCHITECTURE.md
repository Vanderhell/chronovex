# Architecture

Chronovex is a small static C99 library organized around four independently compilable timing operations. Each public header declares one operation; each corresponding source file contains its implementation. The library has no hidden state, allocation, thread, operating-system, or external-library dependency.

## Public modules

| Module | Responsibility | State / ownership |
|---|---|---|
| Duration scaling | Checked rational scaling with nearest-even rounding | Stateless; caller supplies the output pointer |
| Modular elapsed time | Unsigned difference for a wrapping 32-bit counter | Stateless; caller owns timer state and wrap assumptions |
| Absolute deadline construction | Checked non-wrapping `uint64_t` addition | Stateless; caller owns the timestamp domain |
| Signed remaining time | Checked difference between absolute timestamps | Stateless; caller consumes output only on success |

The timer-counter module uses modular arithmetic. Absolute deadline modules use checked mathematical timestamps and reject results outside their representation. The two models are intentionally separate.

## Integration boundaries

`Chronovex::Chronovex` is the exported CMake target. Production source files are listed explicitly in CMake and can also be compiled individually for selective integration. Test programs and examples are optional and do not build by default when Chronovex is consumed as a dependency.

Headers expose the project-specific API and C++ linkage guards. Forwarding headers preserve source compatibility for the v0.1 include paths and names; the compatibility layer maps to the new API and does not emit duplicate legacy linker symbols. See the README and changelog before migrating prebuilt consumers.

## Runtime properties

- Fixed-width integer inputs and outputs.
- Deterministic result for a given input.
- Caller-owned output pointers; documented failures preserve output values.
- No heap allocation, static mutable state, or external runtime service.
- Bounded execution with fixed arithmetic operations and no data-dependent loops in production functions.
