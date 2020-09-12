" this is unfinished. 
" it seems a bit complicated to do syntax highlighting in vim correctly

syntax match fn	/fn/
syntax match if /if/
syntax match else /else/
syntax match struct /struct/
syntax match switch /switch/
syntax match case /case/
syntax match for /for/
syntax match while /while/
syntax match loop /loop/

syntax match Type /[A-Z][A-Za-z0-9]*/ contained
syntax match identifier /[a-z][a-zA-Z0-9]*/ contained
syntax match Braces	/({|})/

let b:current_syntax = 'dg'
