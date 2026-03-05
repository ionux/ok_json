# Design Philosophy

## 1) Deterministic resource usage

The parser is deliberately fixed-capacity: token storage is a static array in `OkJsonParser`, and size limits are compile-time constants. This avoids heap fragmentation and makes worst-case memory use easy to reason about.

## 2) Minimal dependency surface

The project intentionally avoids relying on common libc helpers for core parsing logic (`ctype`, `string`, `stdio` in parser core). Instead it uses local helper functions for character classification and literal matching, reducing platform assumptions.

## 3) Safety-minded implementation style

The code and comments emphasize practices aligned with safety-critical expectations:

- explicit type widths,
- conservative null checks,
- bounded scans,
- clear error enums,
- straightforward control flow.

`ok_json.h` also supports opting into `<stdint.h>` via `OK_JSON_USE_STDINT_H`, while otherwise defining fixed-width aliases locally.

## 4) Token stream over full DOM/AST

OK_JSON does not allocate a hierarchical in-memory tree. Instead it emits tokens and lets callers resolve keys and values from token order. This trades flexibility for simplicity and predictable memory.

## 5) Practical parsing coverage

The parser supports common JSON primitives but intentionally does not try to be a full RFC feature-complete engine.

Known practical boundaries include:

- Limited/strict handling around advanced escape forms and unicode sequences.
- Numeric parsing focused on basic integer/decimal forms (not rich exponent handling).
- Getter APIs optimized for key/value retrieval rather than arbitrary query language behavior.

## 6) Clear diagnostics via error codes

The `OkjError` enum provides explicit failure reasons (syntax, max lengths, token overflow, etc.) so callers can map parser outcomes to system-level responses.

## Out of scope (current direction)

Based on repository documentation and roadmap files, the project currently defers:

- full MISRA certification/toolchain analysis,
- deeper structural metadata tracking beyond the present token model,
- broader JSON edge-case support expected from heavyweight parsers.
