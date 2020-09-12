
" https://vim.fandom.com/wiki/Creating_your_own_syntax_files
" this is unfinished. 
" it seems a bit complicated to do syntax highlighting in vim correctly


syntax match basicType /[A-Z][A-Za-z0-9]*/  
syntax match identifier /[a-z][a-zA-Z0-9]*/ 
syntax match Braces	/({|})/

syn match basicNumber /[-+]?\d+/
syn match basicString /"[^"]*"/

syntax keyword basicLanguageKeywords fn if else struct switch case for while loop return

syntax region basicBlock start="{" end="}" fold 

"syntax region multilineComment start="/*" end="*/" fold
syntax match singlelineComment /\/\/.*\n/

let b:current_syntax = 'dg'
