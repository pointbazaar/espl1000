%{
	#include <stdio.h>
	
	//for the token key macros
	#include "../parser/main/commandline/TokenKeys.h" 

	int line_no = 1;
%}

%%

fn 	printf("%d\n", FN);
if	printf("%d\n", IF);
else	printf("%d\n", ELSE);
while	printf("%d\n", WHILE);
return	printf("%d\n", RETURN);
struct	printf("%d\n", STRUCT);
loop	printf("%d\n", LOOP);
for	printf("%d\n", FOR);
break	printf("%d\n", BREAK);
in	printf("%d\n", IN);
switch	printf("%d\n", SWITCH);
case	printf("%d\n", CASE);

"\n"	printf("%d %d\n", LINE_NO, ++line_no);

"{"	printf("%d\n", LCURLY);
"}"	printf("%d\n", RCURLY);
"["	printf("%d\n", LBRACKET);
"]"	printf("%d\n", RBRACKET);
"("	printf("%d\n", LPARENS);
")"	printf("%d\n", RPARENS);

[0-9]\.[0-9]+		printf("%d %s\n", FLOATING, yytext);
[0-9]*			printf("%d %s\n", INTEGER, yytext);
'.'			printf("%d %s\n", CCONST, yytext);
\".*\"			printf("%d %s\n", STRINGCONST, yytext);
(true|false)		printf("%d %s\n", BCONST, yytext);

[a-z][a-zA-Z0-9]*	printf("%d %s\n", ID, yytext);
[A-Z][a-zA-Z0-9]*	printf("%d %s\n", TYPEIDENTIFIER, yytext);
\?T[0-9]+		printf("%d %s\n", TPARAM, yytext);

"(->|~>)"	printf("%d\n", ARROW);
"#"		printf("%d\n", ANYTYPE);
"~"		printf("%d\n", WAVE);
;		printf("%d\n", SEMICOLON);
".."		printf("%d\n", RANGEOP);
"."		printf("%d\n", STRUCTMEMBERACCESS);
"="		printf("%d\n", ASSIGNOP);

(\+|\-|\*|\/|\!|<|>|<=|>=)	printf("%d\n", OPKEY);

%%
