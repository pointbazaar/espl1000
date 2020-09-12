
" https://vim.fandom.com/wiki/Creating_your_own_syntax_files
" this is unfinished. 
" it seems a bit complicated to do syntax highlighting in vim correctly

syntax region multilineComment start="/\*" end="\*/" fold

syn match basicOperator /[\.\[\]+\-\~!\*\/%<>=&\^|,]/


syntax match Braces display /({|})/
syn match Parens /(\(|\))/

syn match basicNumber /[-+]?\d+/
syn match basicString /"[^"]*"/


syntax match basicType /[A-Z][A-Za-z0-9]*/  
syntax match basicType2 /(\[)*[A-Z][A-Za-z0-9]*(\])*/  
syntax match basicIdentifier /[a-z][a-zA-Z0-9]*/ 

syntax keyword basicLanguageKeywords fn if else struct switch case for while loop return break

syn keyword basicBool true false

syntax match singlelineComment "//.*"

"syntax region basicBlock start="{" end="}" fold 

let b:current_syntax = 'dg'
