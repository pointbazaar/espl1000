![logo](https://raw.githubusercontent.com/pointbazaar/dragon-lexer/master/img/dragon-logo.png)

# Dragon-Parser
[![Build Status](https://travis-ci.org/pointbazaar/dragon-parser.svg?branch=master)](https://travis-ci.org/pointbazaar/dragon-parser)
[![CodeFactor](https://www.codefactor.io/repository/github/pointbazaar/dragon-parser/badge)](https://www.codefactor.io/repository/github/pointbazaar/dragon-parser)
[![GitHub license](https://img.shields.io/github/license/pointbazaar/dragon-parser.svg)](https://github.com/pointbazaar/dragon-parser/blob/master/LICENSE)
[![Open Source Love png2](https://badges.frapsoft.com/os/v2/open-source.png?v=103)](https://github.com/ellerbrock/open-source-badges/)

This is the parser for [smalldragon](https://github.com/pointbazaar/smalldragon), 
a simple, statically typed, compiled programming language.

You can use it to build your own
compiler/interpreter/typechecker/static analyzer/... 
for that language.

## Grammar that it recognizes

```
ArrayConstant ::= '[' expression (',' expression)* ']'

AST ::= namespace+

//the name of the namespace is the filename.
namespace ::=  structDecl* method*

structDecl ::= 'struct' SimpleType '{' structMemberDecl (',' structMemberDecl)* '}'

structMemberDecl ::= Type identifier

SubroutineType ::= '(' Type* ')' arrow Type
SimpleType ::= upperCaseLetter alphanumeric* | anyTypeToken
BasicTypeWrapped ::= SimpleType | '(' SubroutineType ')'

TypeParameter ::= '?T' ( 0 | 1 | ... )

ArrayType :: = '[' Type ']'

Type ::= BasicTypeWrapped | TypeParameter | ArrayType

simplevariable ::= identifier ('[' expression ']')?
variable ::= simplevariable ('.' variable)*

method ::= 'fn' identifier '(' declaredArgument* ')' arrow Type '{' statement* '}'

declaredArgument ::= Type identifier? 

//the optional identifier is there to support for example the 
//" (?T0,PInt size)~>[?T0] newarray{...} " builtin subroutine.
//in that case we receive just the type as a parameter, not an instance of it.

statement ::=   ( methodCall ';' )
                | whileStatement 
                | ifStatement 
                | returnStatement 
                | assignmentStatement

methodCall ::= identifier '(' (expression (, expression)*)?  ')'

//currently not in use
loopStatement ::= 'loop' expression '{' statement* '}'

whileStatement ::= 'while' expression '{' statement* '}'

ifStatement ::= 'if' expression '{' statement* '}' 
				( 'else' '{' statement* '}' )?
				
returnStatement ::= 'return' expression? ';'

assignmentStatement ::= Type? variable '=' expression ';'

Term ::= ArrayConstant 
		| boolConstant 
		| integerConstant 
		| charConstant 
		| methodCall 
		| '(' expression ')' 
		| variable

expression ::= Term (op Term)*

```

## Simplifications performed
- it transforms a string constant into an array

```
expression ::= Term (op Term)*

//is transformed into:

expression ::= Term (op Term)+

//with operator precedence recognized
```


