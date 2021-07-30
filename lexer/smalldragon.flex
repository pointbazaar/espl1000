%{
	#include <stdio.h>
	#include <stdbool.h>
	#include "driver.h"
	#include "lexer_flags.h"
	#include "test.h"
	
	//for the token key macros
	#include "../token/TokenKeys.h" 

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
<single_line_comment>\n	    out2(LINE_NO, ++line_no); BEGIN(INITIAL);


fn 			out(FN, yytext);
if			out(IF, yytext);
else		out(ELSE, yytext);
while		out(WHILE, yytext);
return		out(RETURN, yytext);
struct		out(STRUCT, yytext);
loop		out(LOOP, yytext);
for			out(FOR, yytext);
break		out(BREAK, yytext);
continue 	out(CONTINUE, yytext);
in			out(IN, yytext);

externc     out(EXTERNC, yytext);
#include\ (<[\.a-zA-Z0-9\/_-]+\.h>|\"[\.a-zA-Z0-9\/_-]+\.h\")    out(INCLUDE_DECL, yytext);

switch		out(SWITCH, yytext);
case		out(CASE, yytext);

try			out(TRY, yytext);
catch		out(CATCH, yytext);
throws		out(THROWS, yytext);
throw		out(THROW, yytext);

int|int8|int16|int32|int64	out(TYPEID_PRIMITIVE_INT, yytext);

uint|uint8|uint16|uint32|uint64	out(TYPEID_PRIMITIVE_INT, yytext);

bool 	out(TYPEID_PRIMITIVE_BOOL, yytext);
char	out(TYPEID_PRIMITIVE_CHAR, yytext);
float	out(TYPEID_PRIMITIVE_FLOAT, yytext);

"\n"	out2(LINE_NO, ++line_no);

"{"		out(LCURLY, yytext);
"}"		out(RCURLY, yytext);
"["		out(LBRACKET, yytext);
"]"		out(RBRACKET, yytext);
"("		out(LPARENS, yytext);
")"		out(RPARENS, yytext);

(0|[1-9][0-9]*)\.[0-9]+		out(FLOATING, yytext);
[0-9]+			out(INTEGER, yytext);
0x[0-9a-fA-F]+	out(HEXCONST, yytext);
0b[0-1]+		out(BINCONST, yytext);
'\\?.'			out(CCONST, yytext);
\".*\"			out(STRINGCONST, yytext);


true			out(BCONST_TRUE, yytext);
false			out(BCONST_FALSE, yytext);


@halts			out(ANNOT_HALTS, yytext);
@private		out(ANNOT_PRIVATE, yytext);
@public			out(ANNOT_PUBLIC, yytext);
@deprecated		out(ANNOT_DEPRECATED, yytext);

[a-z][a-zA-Z0-9_]*	out(ID, yytext);
[A-Z_][a-zA-Z0-9_]*	out(TYPEID, yytext);
\?T[0-9]+			{
						int num = 0;
						//printf("yytext=%s\n",yytext);
						sscanf(yytext, "?T%d", &num);
						//printf("num=%d\n", num);
						out2(TPARAM, num);
					}

(->|~>)	out(ARROW, yytext);
"#"		out(ANYTYPE, yytext);
;		out(SEMICOLON, yytext);
,		out(COMMA, yytext);
".."	out(RANGEOP, yytext);
"."		out(STRUCTMEMBERACCESS, yytext);

=					out(ASSIGNOP, yytext);
\+=|-=|\*=|\/=|\%=	out(ASSIGNOP, yytext);
&=|\|=				out(ASSIGNOP, yytext);
"<<="				out(ASSIGNOP, yytext);
">>="				out(ASSIGNOP, yytext);

"<<"				out(OPKEY_BITWISE, yytext);
">>"				out(OPKEY_BITWISE, yytext);
(~|\^|\||\&)		out(OPKEY_BITWISE, yytext);

\+\+				out_plus_plus(ASSIGNOP, INTEGER);
\-\-				out_minus_minus(ASSIGNOP, INTEGER);

(\+|\-|\*|\/|%)		out(OPKEY_ARITHMETIC, yytext);

(==|!=|<=|>=|<|>)	out(OPKEY_RELATIONAL, yytext);

(&&|\|\||\!)		out(OPKEY_LOGICAL, yytext);

" "*		 //blank
"\t"*		 //tab

%%

int main(int argc, char* argv[]){
	//printf("running lexer...\n");
	
	struct LexerFlags* myargs = 
		handle_arguments(argc, argv);
	
	if(myargs->help){
		lexer_print_help();
	}
	
	if(myargs->version){
		printf("dragon-lexer v0.8.1\n");
		free(myargs);
		exit(0);
	}
		
	if(myargs->test){
		lexer_test_all();
		exit(0);
	}
	
	if(myargs->filename == NULL) {
		printf("expecte a filename of the file to tokenize\n");
		exit(1);
	}
	
	char* filename = myargs->filename;
	
	if(myargs->debug){
		printf("input file: %s\n", filename);
	}
	
	//configure input source
	yyin = fopen(filename, "r");
	
	char* buffer = lexer_make_tkn_filename(filename, myargs->debug);
	
	//configure output file
	//NECESSARY (for FLEX)
	outFile = fopen(buffer, "w");
	
	//full buffering for better performance
	setvbuf(outFile, NULL, _IOFBF, BUFSIZ);
	
	yylex();
	
	fclose(yyin);
	free(myargs);
	
	fclose(outFile);
	yylex_destroy();
	free(buffer);
}
