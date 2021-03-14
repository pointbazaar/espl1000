<img src="https://raw.githubusercontent.com/pointbazaar/smalldragon/dev/lexer/img/dragon-logo.svg" width="100" height="100"/>

# Dragon-Parser

This is the parser for smalldragon.
You can use it to build your own compiler/interpreter/typechecker/static analyzer/... for that language.

## Grammar that it recognizes

```
ArrayConstant ::= '[' expression (',' expression)* ']'

AST ::= namespace+

namespace ::=  structDecl* method*

structDecl ::= 'struct' SimpleType '{' structMemberDecl* '}'

method ::= 'fn' identifier '(' declaredArgument* ')' arrow Type stmtblock

//--------------------------------

structMemberDecl ::= Type identifier ';'

simplevariable ::= identifier ('[' expression ']')*

variable ::= simplevariable ('.' variable)?

stmtblock ::= '{' statement* '}'

declaredArgument ::= Type identifier? 

//the optional identifier is there to support for example the 
//" (?T0,uint size)~>[?T0] newarray{...} " builtin subroutine.
//in that case we receive just the type as a parameter, not an instance of it.

// ----- TYPE RELATED ------------------------------------------

SubroutineType ::= '(' Type* ')' arrow Type

StructType ::= upperCaseLetter alphanumeric* ('<'TypeParameter+'>')? | anyTypeToken

PrimitiveType ::= int | float | uint | char | bool | uint8 | ...

SimpleType ::= PrimitiveType | StructType 

BasicTypeWrapped ::= SimpleType | '(' SubroutineType ')'

TypeParameter ::= '?T' ( 0 | 1 | ... )

ArrayType ::= '[' Type ']'

Type ::= BasicTypeWrapped | TypeParameter | ArrayType

// ------------------------------------------------------------

statement ::=   ( methodCall ';' )
				| whileStmt 
				| ifStmt
				| returnStmt
				| assignmentStmt
				| loopStmt
				| breakStmt
				| continueStmt
				| forStmt
				| switchStmt


// -------- STATEMENTS --------------------

methodCall ::= identifier '(' (expression (, expression)*)?  ')'

breakStmt ::= 'break' ';'

continueStmt ::= 'continue' ';'

loopStmt ::= 'loop' expression stmtblock

whileStmt ::= 'while' expression stmtblock

forStmt ::= 'for' identifier 'in' Range stmtblock

ifStmt ::= 'if' expression stmtblock
		( 'else' stmtblock )?
				
returnStmt ::= 'return' expression? ';'

assignmentStmt ::= Type? variable '=' expression ';'

switchStmt ::= 'switch' variable '{' CaseStmt* '}'

CaseStmt ::= 'case' (BoolConst | IntConst | CharConst) StmtBlock?

// --- END STATEMENTS ----------------------

Range ::= Expr '..' Expr

UnOpTerm ::= op Term //op must be a unary operator like '!', '~'

Term ::= ArrayConstant 
		| boolConstant 
		| integerConstant 
		| hexConstant
		| charConstant 
		| StringConstant
		| methodCall 
		| '(' expression ')' 
		| variable

expression ::= UnOpTerm (op UnOpTerm)*

// ---- END OF GRAMMAR DESCRIPTION -------------------------------

Simplifications performed:
```
expression ::= UnOpTerm (op UnOpTerm)*

//is transformed into:

expression ::= UnOpTerm (op UnOpTerm)+

//with operator precedence recognized
```


