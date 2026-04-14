# Security Policy

## Overview

Security matters for `ok_json`.

This project is intended to be a small, portable, well-tested JSON parser written in C99, with a strong focus on correctness, robustness, bounded behavior, and suitability for embedded and safety-critical use cases. Because it processes untrusted input, reports of memory-safety issues, undefined behavior, parser confusion, denial-of-service risks, and other security-relevant defects are taken seriously.

## Supported Versions

Security fixes are applied to the most current development line.

| Version | Supported |
|---------|-----------|
| `main`  | Yes |
| Older commits / snapshots | No |

Until a stable release process is established, please assume that only the current `main` branch is supported for security fixes.

## Reporting a Vulnerability

Please do **not** open a public GitHub issue for a suspected security vulnerability.

Instead, use one of the following private reporting methods:

- GitHub private vulnerability reporting / GitHub Security Advisory
- Contact me privately through GitHub and request a confidential disclosure channel

Please include as much of the following as possible:

- A clear description of the issue
- Affected commit, branch, tag, or release
- Exact input or proof of concept
- Build environment and toolchain details
- Whether the issue was observed with ASan, UBSan, Valgrind, fuzzing, Frama-C, or another tool
- Impact assessment, if known
- Suggested fix, if you have one

Reports that include a minimal reproducer are especially helpful.

## What to Report

Examples of security-relevant issues include:

- Out-of-bounds reads or writes
- Use-after-free, double free, or invalid free
- Undefined behavior with security impact
- Integer overflow or underflow affecting bounds or parser state
- Parser state corruption
- Inputs that bypass documented size, depth, token, or structural limits
- Crashes, hangs, or excessive resource consumption caused by malformed input
- Any case where malformed or adversarial input causes behavior outside the documented contract

If you are unsure whether something is “just a bug” or a security issue, report it privately first.

## What Is Usually Not a Security Issue

The following are usually ordinary bugs unless they have a realistic security impact:

- Pure RFC 8259 compliance disagreements with no memory-safety or trust-boundary consequence
- Documentation mistakes
- Build or style issues
- Cosmetic test failures
- Problems caused solely by violating documented API preconditions without any resulting weakness in the library’s own bounds enforcement

That said, if a caller can supply a valid pointer and valid declared length and the parser still reads or writes beyond that bound, that is security-relevant.

## Disclosure Process

My general process is:

1. Acknowledge receipt of the report
2. Reproduce and validate the issue
3. Assess scope and impact
4. Prepare and test a fix
5. Publish a coordinated fix and advisory when appropriate

I will try to acknowledge initial reports within **7 days**.

Please understand that this is an independently maintained project, so exact timelines may vary.

## Coordinated Disclosure

Please allow reasonable time for investigation and remediation before public disclosure.

A good default is **90 days** from initial private report, though issues may be resolved faster or slower depending on severity, complexity, and maintainer availability.

If a report is confirmed, I may credit the reporter in release notes or an advisory unless the reporter prefers to remain anonymous.

## Safe Harbor

I will not take action against researchers who:

- Act in good faith
- Avoid privacy violations, data destruction, and service disruption
- Do not exploit an issue beyond what is reasonably necessary to demonstrate it
- Do not publicly disclose the issue before a fix is available or coordination has concluded

Please keep testing focused on this repository and avoid any activity that could impact third parties.

## Security-Related Quality Measures

This project uses multiple quality and robustness measures intended to help surface defects early, including:

- Unit tests
- Static analysis
- Dynamic memory checking
- Coverage-guided fuzzing
- Formal specification / proof work where applicable

These measures improve confidence, but they do **not** guarantee the absence of security vulnerabilities. Responsible reports are welcome even if the issue appears to contradict an existing test, sanitizer result, or proof assumption.

## Hardening Expectations

Users of `ok_json` should still treat it as a normal C library and apply standard defensive practices, including:

- Passing valid buffers and accurate lengths
- Respecting documented API contracts
- Compiling with warnings enabled
- Using sanitizers and static analysis in their own integration environment when possible
- Revalidating behavior after changing parser limits or compile-time configuration

## Thanks

Thank you for taking the time to report security issues responsibly. Careful reports, proof-of-concept cases, and well-scoped reproductions materially improve the safety and quality of this project.
