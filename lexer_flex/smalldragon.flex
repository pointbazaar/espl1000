%{
	#include <stdio.h>
	
	//for the token key macros
	#include "../parser/main/commandline/TokenKeys.h" 

	int line_no = 1;
%}

%%

fn 	printf("%d %s\n", FN, yytext);
if	printf("%d %s\n", IF, yytext);
else	printf("%d %s\n", ELSE, yytext);
while	printf("%d %s\n", WHILE, yytext);
return	printf("%d %s\n", RETURN, yytext);
struct	printf("%d %s\n", STRUCT, yytext);
loop	printf("%d %s\n", LOOP, yytext);
for	printf("%d %s\n", FOR, yytext);
break	printf("%d %s\n", BREAK, yytext);
in	printf("%d %s\n", IN, yytext);
switch	printf("%d %s\n", SWITCH, yytext);
case	printf("%d %s\n", CASE, yytext);

"\n"	printf("%d %d\n", LINE_NO, ++line_no);

"{"	printf("%d %s\n", LCURLY, yytext);
"}"	printf("%d %s\n", RCURLY, yytext);
"["	printf("%d %s\n", LBRACKET, yytext);
"]"	printf("%d %s\n", RBRACKET, yytext);
"("	printf("%d %s\n", LPARENS, yytext);
")"	printf("%d %s\n", RPARENS, yytext);

[0-9]\.[0-9]+		printf("%d %s\n", FLOATING, yytext);
[0-9]*			printf("%d %s\n", INTEGER, yytext);
'.'			printf("%d %s\n", CCONST, yytext);
\".*\"			printf("%d %s\n", STRINGCONST, yytext);
(true|false)		printf("%d %s\n", BCONST, yytext);

[a-z][a-zA-Z0-9]*	printf("%d %s\n", ID, yytext);
[A-Z][a-zA-Z0-9]*	printf("%d %s\n", TYPEIDENTIFIER, yytext);
\?T[0-9]+		printf("%d %s\n", TPARAM, yytext);

(->|~>)	printf("%d %s\n", ARROW, yytext);
"#"		printf("%d %s\n", ANYTYPE, yytext);
"~"		printf("%d %s\n", WAVE, yytext);
;		printf("%d %s\n", SEMICOLON, yytext);
,		printf("%d %s\n", COMMA, yytext);
".."		printf("%d %s\n", RANGEOP, yytext);
"."		printf("%d %s\n", STRUCTMEMBERACCESS, yytext);
"="		printf("%d %s\n", ASSIGNOP, yytext);

(\+|\-|\*|\/|\!|<|>|<=|>=)	printf("%d %s\n", OPKEY, yytext);

" "*		 //blank
"\t"*		 //tab

%%
