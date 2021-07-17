# Dragon-Parser

This is the parser for smalldragon.
You can use it to build your own 
compiler/interpreter/typechecker/static analyzer/... 
for it.

## Grammar that it recognizes

```
AST ::= Namespace+

Namespace ::=  ExternC* StructDecl* Method*

StructDecl ::= 'struct' SimpleType '{' StructMember* '}'

MethodDecl ::= 'fn' Identifier '(' DeclArg* ')' arrow Type ('throws')?

ExternC ::= 'externc' MethodDecl StringConst 

Method ::= MethodDecl StmtBlock

//--------------------------------

ArrayConstant ::= '[' Expr (',' Expr)* ']'

StructMember ::= Type Identifier ';'

SimpleVar ::= Identifier ('[' Expr ']')*

Variable ::= SimpleVar ('.' Variable)?

StmtBlock ::= '{' Stmt* '}'

DeclArg ::= Type Identifier? 

//the optional identifier is there to support for example the 
//" (?T0,uint size)~>[?T0] newarray{...} " builtin subroutine.
//in that case we receive just the type as a parameter, not an instance of it.

// ----- TYPE RELATED ------------------------------------------

SubrType ::= '(' Type* ')' arrow Type ('throws')?

StructType ::= upperCaseLetter alphanumeric* ('<'TypeParam+'>')? | anyTypeToken

PrimitiveType ::= int | float | uint | char | bool | uint8 | ...

SimpleType ::= PrimitiveType | StructType 

BasicType ::= SimpleType | '(' SubrType ')'

TypeParam ::= '?T' ( 0 | 1 | ... )

ArrayType ::= '[' Type ']'

Type ::= BasicType | TypeParam | ArrayType

// ------------------------------------------------------------

Stmt ::=   ( Call ';' )
			| whileStmt 
			| ifStmt
			| returnStmt
			| assignmentStmt
			| loopStmt
			| 'break'    ';'
			| 'continue' ';'
			| 'throw'    ';'
			| forStmt
			| switchStmt
			| TryCatchStmt


// -------- STATEMENTS --------------------

Call ::= Identifier '(' (Expr (, Expr)*)?  ')'

loopStmt ::= 'loop' Expr StmtBlock

whileStmt ::= 'while' Expr StmtBlock

forStmt ::= 'for' Identifier 'in' Range StmtBlock

ifStmt ::= 'if' Expr StmtBlock
		( 'else' StmtBlock )?
				
returnStmt ::= 'return' Expr? ';'

assignmentStmt ::= Type? Variable '=' Expr ';'

switchStmt ::= 'switch' Expr '{' CaseStmt* '}'

CaseStmt ::= 'case' ConstValue StmtBlock?

TryCatchStmt ::= 'try' StmtBlock 'catch' StmtBlock

// --- END STATEMENTS ----------------------

//NEW
ConstValue ::= BoolConst | IntConst | BinConst | HexConst 
                        | CharConst | FloatConst

Range ::= Expr '..' Expr

UnOpTerm ::= Op Term //op must be a unary operator like '!', '~'

Term ::= ArrayConst
		| ConstValue
		| StringConst
		| Call 
		| '(' Expr ')' 
		| Variable
		| Lambda

Expr ::= UnOpTerm (Op UnOpTerm)*

Lambda ::= '(' Identifier (,Identifier)* ')' arrow Expr

// ---- END OF GRAMMAR DESCRIPTION -------------------------------

Simplifications performed:
```
Expr ::= UnOpTerm (op UnOpTerm)*

//is transformed into:

Expr ::= UnOpTerm (op UnOpTerm)+

//with operator precedence recognized
```


