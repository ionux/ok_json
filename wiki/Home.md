# OK_JSON Wiki

Welcome to the project wiki for **OK_JSON**, a small, dependency-minimal JSON parser for C99 systems.

This wiki is intended to serve as a practical replacement for a hosted project wiki and gathers architecture, API, usage, and development guidance in one place.

## Pages

- [Project Overview](./Project-Overview.md)
- [Design Philosophy](./Design-Philosophy.md)
- [API Reference](./API-Reference.md)
- [Usage Guide](./Usage-Guide.md)
- [Development, Testing, and CI](./Development-and-Testing.md)

## Who this library is for

OK_JSON is built for developers who need:

- **Portable C99** code with no heavy runtime assumptions.
- A **small parser footprint** with fixed memory usage.
- A parser approach that is straightforward to review for embedded and safety-minded workflows.

If your project needs full JSON features (extensive escape/unicode handling, rich numeric formats, dynamic AST allocation), this library may be intentionally too strict/minimal. See the “Out of Scope” section in [Design Philosophy](./Design-Philosophy.md).
