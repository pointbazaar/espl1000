" this is unfinished. 
" it seems a bit complicated to do syntax highlighting in vim correctly

syntax match fn	/fn/
syntax match struct	/struct/
syntax match Type /[A-Z][A-Za-z0-9]*/ contained
syntax match identifier /[a-z][a-zA-Z0-9]*/ contained
syntax match Braces	/({|})/

let b:current_syntax = 'dragon'
