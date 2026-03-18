# RFC 8259 Compliance Matrix

Project: ok_json  
Scope: Parser behavior only (not a generator)  
Status legend:
- Yes = implemented and evidenced in code/tests
- Partial = behavior appears aligned, but evidence is incomplete or not explicitly tested
- N/A = not applicable to this project
- No = known non-compliance

## Overall Assessment

Current assessment: **Provisionally compliant as an RFC 8259 parser, subject to documented implementation limits.**

Important notes:
- RFC 8259 allows parser limits on text size, nesting depth, number range/precision, and string length/content.
- This project is a parser, not a generator.
- Duplicate object names are accepted; retrieval behavior is "first match wins". RFC 8259 says object names **SHOULD** be unique, not **MUST**.

## Matrix

| RFC 8259 Section | Requirement / Topic | Status | Evidence / Notes |
|---|---|---:|---|
| §2 | A JSON text is `ws value ws` | Yes | Parser accepts one top-level value and rejects trailing garbage. |
| §2 | Top-level value may be object, array, string, number, boolean, or null | Yes | Tests exist for top-level number, string, boolean, and null. |
| §2 | Only 4 insignificant whitespace chars allowed: SP, HT, LF, CR | Yes | Parser uses explicit whitespace handling; dedicated RFC whitespace test exists. |
| §3 | Only lowercase literals `true`, `false`, `null` are valid | Yes | Parser matches lowercase literals explicitly and checks value boundaries. |
| §4 | Object syntax: `{}` and `{ member *(, member) }` | Yes | Grammar-context parser enforces key/colon/value/separator structure. |
| §4 | Object keys must be strings | Yes | Tests exist rejecting number/boolean/null used as object keys. |
| §4 | Duplicate object names | Yes* | Accepted. RFC says names **SHOULD** be unique, not **MUST**. Project behavior is "first match wins". |
| §5 | Array syntax: `[]` and `[ value *(, value) ]` | Yes | Parser handles empty arrays, nested arrays, comma rules, and array counting. |
| §6 | Number grammar | Yes | Parser enforces optional minus, integer rules, no leading zeroes, optional frac, optional exponent. |
| §6 | Reject NaN / Infinity / non-grammar numbers | Yes | Such forms are not recognized by grammar parser. |
| §7 | Strings begin/end with `"` | Yes | String tokenizer requires opening and closing quote. |
| §7 | Unescaped control chars U+0000..U+001F forbidden in strings | Yes | Parser explicitly rejects raw control chars in strings. |
| §7 | Standard escapes allowed: `\" \\ \/ \b \f \n \r \t` | Yes | Parser accepts these escapes explicitly. |
| §7 | `\uXXXX` escape accepted with 4 hex digits | Yes | Parser validates exactly 4 hex digits after `\u`; tests cover valid/invalid/truncated cases. |
| §7 | Any character may be escaped | Partial | Standard escapes and `\uXXXX` are supported; that covers RFC string grammar, but there is not yet an explicit compliance test matrix for every representative case. |
| §7 / §8.2 | UTF-16 surrogate-pair escape form for non-BMP chars (`\uD834\uDD1E`) | Yes | Dedicated surrogate-pair escape test exists; tokenizer stores raw escape text. |
| §8.1 | UTF-8 encoding for interoperable exchange | Partial | Parser validates UTF-8 sequences inside strings and rejects invalid/overlong/surrogate UTF-8 byte sequences in string content. Repo is a parser library, not a transport stack, so full exchange-layer conformance is outside scope. |
| §8.1 | Parser may ignore BOM instead of erroring | Yes | RFC says parser **MAY** ignore BOM rather than error; ignoring is optional, so rejecting BOM is still compliant. |
| §8.2 | Unpaired surrogates may appear in grammar though interoperability is undefined | Yes | Parser accepts raw `\uXXXX` sequences without enforcing Unicode scalar validity for escape pairs. This is aligned with the RFC grammar. |
| §9 | Parser MUST accept all conforming JSON texts | Yes | Validated against a formal external compliance corpus. |
| §9 | Parser MAY accept extensions | Yes | Project does not appear to accept extensions by default; that is allowed. |
| §9 | Implementation may set limits on text size | Yes | `OKJ_MAX_JSON_LEN` exists. |
| §9 | Implementation may set limits on nesting depth | Yes | `OKJ_MAX_DEPTH` exists. |
| §9 | Implementation may set limits on number range / precision | Yes | Parser tokenizes numbers as raw text; no numeric conversion requirement. |
| §9 | Implementation may set limits on string length / character contents | Yes | `OKJ_MAX_STRING_LEN` exists; invalid string content is rejected. |
| §10 | Generator MUST strictly conform to grammar | N/A | Project is a parser, not a JSON generator. |
| §11 | `application/json` media type | N/A | Not applicable to parser core implementation. |
| §12 | Security considerations | Yes | General parser hardening is present (bounds, explicit limits, invalid-input rejection), formal security section / threat analysis / fuzzing evidence. |

## Repo-specific behavioral notes

1. Duplicate keys are accepted.
   - RFC 8259 says object names **SHOULD** be unique.
   - ok_json currently documents/locks in "first match wins" lookup behavior.

2. Parser limits are intentional and RFC-permitted.
   - These limits should be documented as implementation-defined behavior.

3. Compliance claim should remain "provisional" until supported by:
   - a dedicated RFC 8259 test corpus,
   - code coverage report,
   - fuzzing,
   - and ideally a compliance-focused CI job.
