# Assembly Grammar

The assembly language grammar description. The expression `{ expr }` is zero
or more of `expr`, `[ expr ]` is an optional rule, and `"keyword" | ATOM`
are the atomics of the grammar.

```
module -> { include } { sections }

comment -> COMMENT

include     -> "include" file_path [ comment ] LINE_RETURN
file_path   -> IDENTIFIER

section         -> "section" section_label body LINE_RETURN
section_label   -> IDENTIFIER

body        -> { statement }
statement   -> label | instr | comment

label       -> label_name ":" [ comment ] [ LINE_RETURN ]
label_name  -> IDENTIFIER

instr   -> op { operand } [ comment] LINE_RETURN
op      -> IDENTIFIER
operand -> NUMERIC | CONSTPOOL | IDENTIFIER | STRING
```
