# Lexer Analysis for Markdown Parser

## Scope

This analysis is based on the current lexer/token implementation in:

- `code/include/lexer.hpp`
- `code/include/token.hpp`

Note: there is no `lexer.cpp`/`token.cpp` implementation file at the moment; logic is header-only.

## Current Design Summary

- `Lexer` opens a `FILE*` from path and tokenizes char-by-char using `fgets` with a fixed buffer (`256`).
- `Token::getTypeFromChar` maps a subset of single characters to token types.
- If a char is not mapped, it is accumulated into a `currentWord`, later emitted as `WORD` or `NUMBER`.
- There is special handling for `x`/`X` after `]` to support task-list patterns.

## Lexer Problems

- Resource lifecycle issue: `FILE* sourceFile` is opened but never closed (no destructor, no RAII wrapper).
- Error handling is process-level (`exit(EXIT_FAILURE)` inside constructor), which makes the lexer hard to reuse as a library.
- `tokenize()` is not re-entrant on same object: after one run, file cursor stays at EOF.
- Fixed-size line buffer (`char buffer[256]`) is fragile for long Markdown lines.
- Header-only implementation with `using namespace std;` in headers pollutes global namespace in all translation units.
- No explicit include for C stdio APIs in lexer header despite using `FILE`, `fopen`, `fgets`, `perror`.
- No location tracking (line/column), so parser and diagnostics cannot report useful error positions.
- No EOF token or stream end metadata.

## Token Model Problems

- `TokenType` is mostly single-char driven; Markdown requires many context-sensitive/multi-char constructs.
- Ad-hoc token types (`x`, `X`) encode semantic meaning that belongs more to parsing context than raw lexing.
- Many relevant symbols are not tokenized explicitly (`!`, `=`, `/`, `"`, `'`, `) + punctuation combinations), causing ambiguous `WORD` chunks.
- `NUMBER` detection is only for pure digit strings; mixed forms become `WORD` (may be acceptable, but must be intentional).
- Current model does not represent token spans, making later AST mapping and debugging harder.

## Best Practices Gaps

- Prefer RAII (`std::ifstream`) instead of raw `FILE*`.
- Avoid `using namespace std;` in headers.
- Separate declaration (.hpp) and implementation (.cpp) for compile-time hygiene and maintainability.
- Use exceptions or structured error return instead of calling `exit` in core components.
- Add deterministic testable interfaces:
  - `tokenize(std::string_view input)` for unit tests.
  - `tokenizeFile(path)` as convenience wrapper.
- Add token metadata:
  - start/end offset
  - line/column
- Keep lexer as lexical-only; avoid parser-like context hacks in lexer core.

## Possible Problems During Parsing

- Setext heading underline (`===`/`---`) requires line-level and neighbor-line context; current token stream does not preserve robust block boundaries.
- Fenced code blocks require mode switching: code content should be treated as raw text until closing fence.
- Nested lists and continuation lines depend on indentation semantics; current token stream does not expose indentation levels explicitly.
- Link/image parsing (`[text](url "title")`) needs clear tokenization of delimiters and URL chars; current `WORD` chunks can hide structure.
- HTML blocks/inline HTML are highly context-sensitive and will likely conflict with current generic `WORD` aggregation.
- Escapes (`\*`, `\_`, etc.) require escape-aware behavior to prevent false formatting tokens.
- CRLF handling is incomplete (`\r` may be absorbed into words or unknown chunks).

## Markdown-Specific Lexing Risks

- Task lists: `[x]` logic currently depends on previous token only (`]`), which is brittle for spacing variants and nested contexts.
- URLs: `https://...` often splits inconsistently due missing dedicated slash token strategy.
- Emphasis resolution (`*`, `_`) is context-driven (flanking rules); pure char tokens are insufficient without richer parser rules.
- Code fences: triple backticks/tildes are emitted as repeated single-char tokens, making fence width and language tag detection error-prone.
- Mermaid requirement: detecting ```mermaid fences must be reliable and capture whole raw block; current lexer has no dedicated state for that.

## Recommended Refactor Direction

- Introduce lexer modes:
  - Normal inline/block mode
  - Fenced code mode (raw line capture)
- Replace `x`/`X` token special-casing with generic tokens + parser rule for task markers.
- Add explicit token types for frequently needed delimiters used in Markdown grammar.
- Preserve newline and indentation information in a parser-friendly way.
- Add a `TokenSpan` structure (`line`, `column`, `offset`, `length`).
- Add robust tests for edge cases:
  - long lines
  - CRLF files
  - escaped delimiters
  - nested lists
  - mixed HTML/Markdown
  - fenced mermaid blocks

## Priority Fixes (Practical Order)

1. Resource and API cleanup (RAII, no `exit`, reusable tokenize interface).
2. Position tracking and deterministic token spans.
3. Remove ad-hoc `x`/`X` lexer behavior.
4. Add fence-aware lexing mode and Mermaid block tagging support hooks.
5. Expand token set for Markdown delimiters and improve URL/link tokenization.
6. Add conformance-focused test corpus (including CommonMark-like edge cases).

## Conclusion

The current lexer is a good prototype for exploratory token streams, but it is not yet robust enough for a production-quality Markdown parser with Mermaid support. The biggest issues are resource handling, missing positional/context modeling, and ad-hoc token semantics that will cause parser fragility on real-world Markdown files.
