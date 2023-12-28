# Assembly Grammar

The assembly language grammar description. The expression `{ expr }` is zero
or more of `expr`, `[ expr ]` is an optional rule, and `"keyword" | ATOM`
are the atomics of the grammar.

```
module      -> "module" module_name LINE_RETURN { include } { sections }
module_name -> IDENTIFIER

comment -> COMMENT

include     -> "include" file_path LINE_RETURN
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

# Bytecode File Format

```
dependency:
    
symbol_table:
    [symbol]:       offset into the data table
    resolved:       is the symbol resolved
    const_offset:   offset into the constpool section if not resolved
    data_offset:    offset into the data section if it is resolved
    
data:
    code and data section
constpool:
    [offset]: 
    
comments:
    [offset:comment] 
    
```
