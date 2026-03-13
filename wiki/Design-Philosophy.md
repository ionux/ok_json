# Design Philosophy

## 1) Deterministic resource usage

The parser is deliberately fixed-capacity: token storage is a static array in
`OkJsonParser`, and size limits are compile-time constants.  This avoids heap
fragmentation and makes worst-case memory use easy to reason about.

The nesting depth is also bounded at compile time by `OKJ_MAX_DEPTH` (16),
preventing unbounded stack growth on deeply nested input.

## 2) Minimal dependency surface

The project intentionally avoids relying on common libc helpers for core
parsing logic (`ctype`, `string`, `stdio` in parser core).  Instead it uses
local helper functions for character classification and literal matching,
reducing platform assumptions.

## 3) Safety-minded implementation style

The code and comments emphasize practices aligned with safety-critical
expectations:

- explicit type widths,
- conservative null checks,
- bounded scans,
- clear error enums,
- straightforward control flow.

`ok_json.h` supports opting into `<stdint.h>` via `OK_JSON_USE_STDINT_H`,
while otherwise defining fixed-width aliases locally (per MISRA C2012 Dir 4.6).

## 4) Token stream over full DOM/AST

OK_JSON does not allocate a hierarchical in-memory tree.  Instead it emits
tokens and lets callers resolve keys and values from token order.  This trades
flexibility for simplicity and predictable memory.

## 5) Practical parsing coverage

The parser targets the common JSON constructs used in embedded/telemetry
contexts, and validates input carefully within that scope:

- **Strings**: full escape-sequence validation including `\"`, `\\`, `\/`,
  `\b`, `\f`, `\n`, `\r`, `\t`, and `\uXXXX` (4-hex-digit Unicode escapes,
  with surrogate-pair detection).  Raw multi-byte UTF-8 sequences are also
  validated per the RFC 3629 encoding rules.
- **Numbers**: integer, negative, decimal (`3.14`), and exponent forms
  (`1e10`, `2.5E-3`) are accepted.  Degenerate forms such as a lone `-`,
  leading zeros (e.g. `01`), trailing decimal points, and exponents without
  digits are rejected.
- **Structural validation**: trailing commas, mismatched brackets, extra
  top-level values, and non-string object keys are all detected and rejected
  with specific error codes.

Getter APIs are optimised for key/value retrieval rather than arbitrary query
language behaviour.

## 6) Clear diagnostics via error codes

The `OkjError` enum provides explicit failure reasons (syntax, invalid
character, bracket mismatch, max depth/length/token limits, etc.) so callers
can map parser outcomes to system-level responses.  The full list of codes is
in the [API Reference](./API-Reference.md).

## Out of scope (current direction)

Based on repository documentation and roadmap files, the project currently
defers:

- full MISRA certification/toolchain analysis,
- deeper structural metadata tracking beyond the present token model,
- JSON Pointer / JSONPath query language support,
- number-to-native-type conversion (values are exposed as text slices).
