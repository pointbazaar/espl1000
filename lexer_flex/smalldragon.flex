%{
	#include <stdio.h>
	#include "driver.h"
	#include "lexer_flags.h"
	#include "test.h"
	
	//for the token key macros
	#include "../parser/main/commandline/TokenKeys.h" 

	int line_no = 1;
%}

%x single_line_comment
%x multi_line_comment

%%

\/\/	BEGIN(single_line_comment);
\/\*	BEGIN(multi_line_comment);

<multi_line_comment>[^\*\/]* 	//do nothing
<multi_line_comment>\*\/ 	BEGIN(INITIAL);

<single_line_comment>[^\n]*	//do nothing
<single_line_comment>\n	line_no++; BEGIN(INITIAL);


fn 		out(FN, yytext);
if		out(IF, yytext);
else	out(ELSE, yytext);
while	out(WHILE, yytext);
return	out(RETURN, yytext);
struct	out(STRUCT, yytext);
loop	out(LOOP, yytext);
for		out(FOR, yytext);
break	out(BREAK, yytext);
in		out(IN, yytext);
switch	out(SWITCH, yytext);
case	out(CASE, yytext);

"\n"	out2(LINE_NO, ++line_no);

"{"		out(LCURLY, yytext);
"}"		out(RCURLY, yytext);
"["		out(LBRACKET, yytext);
"]"		out(RBRACKET, yytext);
"("		out(LPARENS, yytext);
")"		out(RPARENS, yytext);

[0-9]\.[0-9]+		out(FLOATING, yytext);
[0-9]*			out(INTEGER, yytext);
'\\?.'			out(CCONST, yytext);
\".*\"			out(STRINGCONST, yytext);
(true|false)		out(BCONST, yytext);

[a-z][a-zA-Z0-9]*	out(ID, yytext);
[A-Z][a-zA-Z0-9]*	out(TYPEIDENTIFIER, yytext);
\?T[0-9]+			out(TPARAM, yytext);

(->|~>)	out(ARROW, yytext);
"#"		out(ANYTYPE, yytext);
"~"		out(WAVE, yytext);
;		out(SEMICOLON, yytext);
,		out(COMMA, yytext);
".."	out(RANGEOP, yytext);
"."		out(STRUCTMEMBERACCESS, yytext);
"="		out(ASSIGNOP, yytext);

(\+|\-|\*|\/|\!|<|>|<=|>=)	out(OPKEY, yytext);

" "*		 //blank
"\t"*		 //tab

%%

int main(int argc, char* argv[]){
	//printf("running lexer...\n");
	
	struct LexerFlags* myargs = 
		handleArguments(argc, argv);
		
	if(myargs->test){
		test_all(myargs->debug);
		exit(0);
	}
	
	if(myargs->filename == NULL) {
		printf("expecte a filename of the file to tokenize\n");
		exit(1);
	}
	
	char* filename = myargs->filename;
	printf("input file: %s\n", filename);
	
	driver();
	
	//configure input source
	yyin = fopen(filename, "r");
	
	char buffer[100];
	sprintf(buffer, "%s.tokens", filename);
	
	//configure output file
	outFile = fopen(buffer, "w");
	
	yylex();
}
