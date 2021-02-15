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

variable ::= simplevariable ('.' variable)*

stmtblock ::= '{' statement* '}'

declaredArgument ::= Type identifier? 

//the optional identifier is there to support for example the 
//" (?T0,PInt size)~>[?T0] newarray{...} " builtin subroutine.
//in that case we receive just the type as a parameter, not an instance of it.

// ----- TYPE RELATED ------------------------------------------

SubroutineType ::= '(' Type* ')' arrow Type

SimpleType ::= upperCaseLetter alphanumeric* ('<'TypeParameter+'>')? | anyTypeToken

BasicTypeWrapped ::= SimpleType | '(' SubroutineType ')'

TypeParameter ::= '?T' ( 0 | 1 | ... )

ArrayType ::= '[' Type ']'

Type ::= BasicTypeWrapped | TypeParameter | ArrayType

// ------------------------------------------------------------

statement ::=   ( methodCall ';' )
                | whileStatement 
                | ifStatement 
                | returnStatement 
                | assignmentStatement
		| loopStatement
		| breakStatement
		| forStatement
		| switchStmt


// -------- STATEMENTS --------------------

methodCall ::= identifier '(' (expression (, expression)*)?  ')'

breakStatement ::= 'break' ';'

loopStatement ::= 'loop' expression stmtblock

whileStatement ::= 'while' expression stmtblock

forStatement ::= 'for' identifier 'in' Range stmtblock

ifStatement ::= 'if' expression stmtblock
		( 'else' stmtblock )?
				
returnStatement ::= 'return' expression? ';'

assignmentStatement ::= Type? variable '=' expression ';'

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


