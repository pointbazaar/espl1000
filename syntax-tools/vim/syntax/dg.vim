
" https://vim.fandom.com/wiki/Creating_your_own_syntax_files
" this is unfinished. 
" it seems a bit complicated to do syntax highlighting in vim correctly


" Vim syntax file
" Language:	smalldragon
" Maintainer:	pointbazaar

syn region multilineComment start="/\*" end="\*/" fold

syn match basicOperator /[\.\[\]+\-\~!\*\/%<>=&\^|,]/

syn match annotation /@[a-z]+/

syn match Braces display /({|})/
syn match Parens /(\(|\))/

syn match basicNumber /[-+]?\d+/
syn match basicString /"[^"]*"/

syn match basicType /[A-Z][A-Za-z0-9]*/  
syn match basicType /(\[)*[A-Z][A-Za-z0-9]*(\])*/  

syn keyword primitiveType int uint 
syn keyword primitiveType int8 int16 int32 int64 
syn keyword primitiveType uint8 uint16 uint32 uint64
syn keyword primitiveType char bool float

syn match basicIdentifier /[a-z][a-zA-Z0-9_]*/ 

syn keyword basicLanguageKeywords fn if else struct switch case 
syn keyword basicLanguageKeywords for while return break in continue

syn keyword todo contained TODO FIXME

syn keyword basicBool true false

syn match singlelineComment "//.*"

"syn region basicBlock start="{" end="}" fold 

let b:current_syntax = 'dg'
