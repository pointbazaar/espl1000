" this is unfinished. 
" it seems a bit complicated to do syntax highlighting in vim correctly

syntax match namespace	/namespace/
syntax match struct	/struct/
syntax match PrimitiveType	/(PInt|NInt|Integer|Float|PFloat|NFloat|Char|Bool)/ contained
syntax match Type /[A-Z][A-Za-z]/ contained
syntax match Braces	/({|})/
syntax match access	"(public|private)" contained
syntax match use	/use .*\.dg/



let b:current_syntax = 'dragon'
