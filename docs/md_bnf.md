# Markdown BNF Grammar Reference

---

## 1. Document Structure

```bnf
<document>   ::= <block>*
<block>      ::= <heading>
               | <paragraph>
               | <blockquote>
               | <fenced-code-block>
               | <indented-code-block>
               | <horizontal-rule>
               | <unordered-list>
               | <ordered-list>
               | <table>
               | <blank-line>
```

---

## 2. Blank Line

```bnf
<blank-line> ::= (<space> | <tab>)* <newline>
<newline>    ::= "\n" | "\r\n" | "\r"
<space>      ::= " "
<tab>        ::= "\t"
```

**Example:**

```
(empty line here)
```

---

## 3. Heading

```bnf
<heading>          ::= <atx-heading> | <setext-heading>

<atx-heading>      ::= <atx-marker> <space>+ <inline>+ <atx-closing>? <newline>
<atx-marker>       ::= "#" | "##" | "###" | "####" | "#####" | "######"
<atx-closing>      ::= <space>+ "#"*

<setext-heading>   ::= <inline>+ <newline> <setext-underline> <newline>
<setext-underline> ::= "="+ | "-"+
```

**Example:**

```markdown
# Heading level 1
## Heading level 2
### Heading level 3
#### Heading level 4
##### Heading level 5
###### Heading level 6

Heading level 1
===============

Heading level 2
---------------
```

---

## 4. Paragraph

```bnf
<paragraph>   ::= <inline-line> (<inline-line>)* <blank-line>
<inline-line> ::= <inline>+ <newline>
```

**Example:**

```markdown
This is the first line of the paragraph.
This second line is still part of the same paragraph.

This is a new paragraph after the blank line.
```

---

## 5. Blockquote

```bnf
<blockquote>      ::= <blockquote-line>+
<blockquote-line> ::= ">" <space>? <block-content> <newline>
<block-content>   ::= <inline>* | <block>
```

**Example:**

```markdown
> This is a blockquote.

> First line of a multi-line blockquote.
> Second line continues here.

> This is level one.
> > This is a nested blockquote.
> > > Triple nesting.

> A blockquote can contain a heading.
> # Heading inside quote
>
> And a paragraph.
```

---

## 6. Fenced Code Block

```bnf
<fenced-code-block> ::= <fence-open> <newline>
                        <code-line>*
                        <fence-close> <newline>

<fence-open>        ::= <backtick-fence> <info-string>?
                      | <tilde-fence> <info-string>?
<backtick-fence>    ::= "```" "`"*
<tilde-fence>       ::= "~~~" "~"*
<fence-close>       ::= <backtick-fence> | <tilde-fence>
<info-string>       ::= <non-newline-char>+
<code-line>         ::= <any-char>* <newline>
```

**Example:**

````markdown
```
plain code block, no language
```

```python
def greet(name):
    return f"Hello, {name}"
```

~~~javascript
const x = 42;
~~~
````

---

## 7. Indented Code Block

```bnf
<indented-code-block> ::= (<indent> <code-line>)+
<indent>               ::= "    " | "\t"
<code-line>            ::= <any-char>* <newline>
```

**Example:**

```markdown
    first line of indented code
    second line of indented code
    function foo() { return 1; }
```

---

## 8. Horizontal Rule

```bnf
<horizontal-rule> ::= <hr-dash> | <hr-star> | <hr-underscore>
<hr-dash>         ::= <space>{0,3} "-" <space>* "-" <space>* "-" (<space>* "-")* <newline>
<hr-star>         ::= <space>{0,3} "*" <space>* "*" <space>* "*" (<space>* "*")* <newline>
<hr-underscore>   ::= <space>{0,3} "_" <space>* "_" <space>* "_" (<space>* "_")* <newline>
```

**Example:**

```markdown
---

***

___

- - -

* * * * *
```

---

## 9. Unordered List

```bnf
<unordered-list> ::= <ul-item>+
<ul-item>        ::= <ul-marker> <space>+ <item-content> <newline>
                     (<blank-line> | <continuation>)*
<ul-marker>      ::= "-" | "*" | "+"
<item-content>   ::= <inline>+ | <block>
<continuation>   ::= <space>{2,} <inline>+ <newline>
```

**Example:**

```markdown
- First item
- Second item
- Third item

* Item with asterisk
+ Item with plus

- Item one
- Item two
  - Nested item A
  - Nested item B
    - Double nested

- First paragraph of item.

  Second paragraph of same item (blank line + indent).

- Another item
```

---

## 10. Ordered List

```bnf
<ordered-list> ::= <ol-item>+
<ol-item>      ::= <ol-marker> <space>+ <item-content> <newline>
                   (<blank-line> | <continuation>)*
<ol-marker>    ::= <digits> "." | <digits> ")"
<digits>       ::= [0-9]+
```

**Example:**

```markdown
1. First item
2. Second item
3. Third item

1. Starting number matters for rendering
3. But subsequent numbers do not need to be sequential
99. Any number works here

1) Parenthesis style
2) Also valid

1. Top-level item
   1. Nested ordered item
   2. Another nested item
2. Back to top level
```

---

## 11. Table (GFM Extension)

```bnf
<table>           ::= <table-header> <newline>
                      <table-separator> <newline>
                      <table-row>+

<table-header>    ::= <table-row>
<table-separator> ::= "|"? <sep-cell> ("|" <sep-cell>)* "|"? <newline>
<sep-cell>        ::= <space>* ":"? "-"+ ":"? <space>*
<table-row>       ::= "|"? <table-cell> ("|" <table-cell>)* "|"? <newline>
<table-cell>      ::= <space>* <inline>* <space>*
```

**Example:**

```markdown
| Name  | Age | City  |
|-------|-----|-------|
| Alice | 30  | Rome  |
| Bob   | 25  | Milan |

| Left | Center | Right |
|:-----|:------:|------:|
| text | text   | text  |

Name  | Score
----- | -----
Alice | 100
Bob   | 95
```

---

## 12. Inline Elements

```bnf
<inline> ::= <plain-text>
           | <bold>
           | <italic>
           | <bold-italic>
           | <inline-code>
           | <strikethrough>
           | <link>
           | <image>
           | <autolink>
           | <hard-line-break>
           | <html-entity>
           | <escaped-char>
```

---

## 13. Bold

```bnf
<bold>         ::= "**" <bold-content> "**"
                 | "__" <bold-content> "__"
<bold-content> ::= (<inline> | <space>)+
```

**Example:**

```markdown
**This is bold text**

__This is also bold__

This sentence has **bold words** inside it.
```

---

## 14. Italic

```bnf
<italic>         ::= "*" <italic-content> "*"
                   | "_" <italic-content> "_"
<italic-content> ::= (<inline> | <space>)+
```

**Example:**

```markdown
*This is italic text*

_This is also italic_

A sentence with *italic words* inside.
```

---

## 15. Bold-Italic

```bnf
<bold-italic> ::= "***" <inline>+ "***"
                | "___" <inline>+ "___"
                | "**_" <inline>+ "_**"
                | "*__" <inline>+ "__*"
```

**Example:**

```markdown
***bold and italic together***

___also bold and italic___

**_mixed delimiter style_**
```

---

## 16. Inline Code

```bnf
<inline-code>          ::= <single-backtick-code> | <double-backtick-code>
<single-backtick-code> ::= "`" <non-backtick-char>+ "`"
<double-backtick-code> ::= "``" <double-bt-content> "``"
<double-bt-content>    ::= (<any-char except "``">)+
```

**Example:**

```markdown
Use `printf()` to print output.

The variable `x` holds the value.

Use `` `backtick` `` inside double backticks.
```

---

## 17. Strikethrough (GFM Extension)

```bnf
<strikethrough> ::= "~~" <inline>+ "~~"
```

**Example:**

```markdown
~~This text is deleted.~~

The price is ~~$100~~ $80.
```

---

## 18. Link

```bnf
<link>               ::= <inline-link> | <reference-link> | <collapsed-ref-link>

<inline-link>        ::= "[" <link-text> "]" "(" <link-destination> <link-title>? ")"
<link-text>          ::= <inline>*
<link-destination>   ::= <uri-chars>+ | "<" <uri-chars>* ">"
<link-title>         ::= <space>+ "\"" <any-char>* "\""
                       | <space>+ "'" <any-char>* "'"
                       | <space>+ "(" <any-char>* ")"

<reference-link>     ::= "[" <link-text> "]" "[" <link-label> "]"
<collapsed-ref-link> ::= "[" <link-label> "]" "[]"
<link-label>         ::= <non-newline-char>+

<link-definition>    ::= "[" <link-label> "]" ":" <space>+ <link-destination>
                         (<space>+ <link-title>)? <newline>
```

**Example:**

```markdown
[OpenAI](https://openai.com)

[Homepage](https://example.com "Visit the homepage")

[link with angle brackets](<https://example.com>)

[reference link][my-ref]

[my-ref]: https://example.com "Optional title"

[collapsed][]

[collapsed]: https://example.com
```

---

## 19. Image

```bnf
<image>    ::= "!" "[" <alt-text> "]" "(" <link-destination> <link-title>? ")"
             | "!" "[" <alt-text> "]" "[" <link-label> "]"
<alt-text> ::= <non-bracket-char>*
```

**Example:**

```markdown
![A cat](https://example.com/cat.jpg)

![Logo](https://example.com/logo.png "Company logo")

![Alt text][img-ref]

[img-ref]: https://example.com/image.png
```

---

## 20. Autolink

```bnf
<autolink>      ::= "<" <absolute-uri> ">"
                  | "<" <email-address> ">"
<absolute-uri>  ::= <scheme> ":" <uri-chars>+
<scheme>        ::= [a-zA-Z] [a-zA-Z0-9+\-.]*
<email-address> ::= <email-local> "@" <email-domain>
<email-local>   ::= [a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+
<email-domain>  ::= <label> ("." <label>)*
<label>         ::= [a-zA-Z0-9-]+
```

**Example:**

```markdown
<https://example.com>

<ftp://files.example.com/data.zip>

<user@example.com>
```

---

## 21. Hard Line Break

```bnf
<hard-line-break>       ::= <trailing-spaces-break> | <backslash-break>
<trailing-spaces-break> ::= <space> <space>+ <newline>
<backslash-break>       ::= "\" <newline>
```

**Example:**

```markdown
Line one ends with two spaces  
Line two starts here.

Line one ends with a backslash\
Line two starts here.
```

---

## 22. Escaped Character

```bnf
<escaped-char>      ::= "\" <ascii-punctuation>
<ascii-punctuation> ::= "!" | "\"" | "#" | "$" | "%" | "&" | "'" | "("
                      | ")" | "*" | "+" | "," | "-" | "." | "/" | ":"
                      | ";" | "<" | "=" | ">" | "?" | "@" | "[" | "\"
                      | "]" | "^" | "_" | "`" | "{" | "|" | "}" | "~"
```

**Example:**

```markdown
\*This is not italic\*

\# This is not a heading

\[This is not a link\]

\`This is not code\`
```

---

## 23. HTML Entity

```bnf
<html-entity>    ::= <named-entity> | <decimal-entity> | <hex-entity>
<named-entity>   ::= "&" <entity-name> ";"
<entity-name>    ::= [a-zA-Z0-9]+
<decimal-entity> ::= "&#" [0-9]+ ";"
<hex-entity>     ::= "&#" ("x" | "X") [0-9a-fA-F]+ ";"
```

**Example:**

```markdown
&amp;   renders as  &
&lt;    renders as  <
&gt;    renders as  >
&copy;  renders as  ©
&#169;  renders as  ©
&#xA9;  renders as  ©
```

---

## 24. Plain Text

```bnf
<plain-text> ::= <plain-char>+
<plain-char> ::= any Unicode character except
                 "*" | "_" | "`" | "[" | "]" | "!" | "<" | ">"
                 | "&" | "\" | <newline>
```

**Example:**

```markdown
This is plain text with no special characters.
```

---

## 25. Summary of Precedence and Disambiguation Rules

```
1. Blank lines separate blocks.
2. A setext heading underline cannot be confused with a list item marker
   if the line before it is not a valid paragraph continuation.
3. A fenced code block fence must be at least 3 characters long.
4. An indented code block cannot interrupt a paragraph.
5. Inline code spans take priority over all other inline constructs.
6. Emphasis delimiters must not be surrounded by Unicode spaces on both sides.
7. Link text may not contain other links (no nesting of links).
8. An image is syntactically identical to a link, prefixed by "!".
9. HTML entities are passed through; unrecognized ones are left literal.
```
