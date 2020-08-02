//standard headers
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

//user headers
#include "tokens.h"
#include "lexer.h"
#include "strutils.h"
#include "states.h"
#include "loop.h"


void free_dfa(short** dfa,int n_states){

	for(short i=0;i<n_states;i++){
		free(dfa[i]);
	}
	free(dfa);
}


	
void init_if_else(short** dfa){
	printf("creating states for 'if' , 'else' \n");

	// -------------- IF ------------------

	//all other chars should get us back to S_IDENTIFIER
	set_transitions_letters(dfa,S_I,S_IDENTIFIER);
	set_transitions_letters(dfa,S_IF,S_IDENTIFIER);

	//TODO: all other non alphanumeric chars should get us 
	//into final state

	dfa[S_I]['f']=S_IF;
	//now for many tokens it would go into a final state

	set_transitions_breaking(dfa,S_I,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_IF,S_IF_FINAL);

	// -------------- ELSE ------------------

	set_transitions_letters(dfa,S_e,S_IDENTIFIER);
	set_transitions_letters(dfa,S_el,S_IDENTIFIER);
	set_transitions_letters(dfa,S_els,S_IDENTIFIER);
	set_transitions_letters(dfa,S_else,S_IDENTIFIER);

	set_transitions_breaking(dfa,S_e,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_el,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_els,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_else,S_else_FINAL);

	dfa[S_e]['l']=S_el;
	dfa[S_el]['s']=S_els;
	dfa[S_els]['e']=S_else;
	
}

void init_while(short** dfa){
	printf("creating states for 'while' \n");
	//all other chars should get us back to S_IDENTIFIER,
	//to parse an identifier instead

	set_transitions_letters(dfa,S_W,S_IDENTIFIER);
	set_transitions_letters(dfa,S_WH,S_IDENTIFIER);
	set_transitions_letters(dfa,S_WHI,S_IDENTIFIER);
	set_transitions_letters(dfa,S_WHIL,S_IDENTIFIER);
	set_transitions_letters(dfa,S_WHILE,S_IDENTIFIER);

	set_transitions_breaking(dfa,S_W,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_WH,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_WHI,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_WHIL,S_IDENTIFIER_FINAL);
	
	set_transitions_breaking(dfa,S_WHILE,S_WHILE_FINAL);

	dfa[S_W]['h']=S_WH;
	dfa[S_WH]['i']=S_WHI;
	dfa[S_WHI]['l']=S_WHIL;
	dfa[S_WHIL]['e']=S_WHILE;
	dfa[S_WHILE][' ']=S_WHILE_FINAL;
}

void init_identifier(short**	 dfa){
	printf("creating states for identifier token \n");

	set_transitions_letters(dfa,S_IDENTIFIER,S_IDENTIFIER);
	set_transitions_braces(dfa,S_IDENTIFIER,S_IDENTIFIER_FINAL);
	set_transitions_digits(dfa,S_IDENTIFIER,S_IDENTIFIER);

	dfa[S_IDENTIFIER]['_']=S_IDENTIFIER;

	dfa[S_IDENTIFIER][' ']=S_IDENTIFIER_FINAL;

	dfa[S_IDENTIFIER]['=']=S_IDENTIFIER_FINAL;
	dfa[S_IDENTIFIER][',']=S_IDENTIFIER_FINAL;
	
	dfa[S_IDENTIFIER]['.']=S_IDENTIFIER_FINAL;

	set_transitions_breaking(dfa,S_IDENTIFIER,S_IDENTIFIER_FINAL);
}


void init_typeidentifier(short** dfa){
	printf("creating states for typeidentifier token \n");

	set_transitions_letters(dfa,S_TYPEIDENTIFIER,S_TYPEIDENTIFIER);
	set_transitions_braces(dfa,S_TYPEIDENTIFIER,S_TYPEIDENTIFIER_FINAL);
	set_transitions_digits(dfa,S_TYPEIDENTIFIER,S_TYPEIDENTIFIER);

	dfa[S_TYPEIDENTIFIER]['_']=S_TYPEIDENTIFIER;

	dfa[S_TYPEIDENTIFIER][' ']=S_TYPEIDENTIFIER_FINAL;

	dfa[S_TYPEIDENTIFIER]['=']=S_TYPEIDENTIFIER_FINAL;
	dfa[S_TYPEIDENTIFIER][',']=S_TYPEIDENTIFIER_FINAL;

	dfa[S_TYPEIDENTIFIER]['-']=S_TYPEIDENTIFIER_FINAL;
	dfa[S_TYPEIDENTIFIER]['+']=S_TYPEIDENTIFIER_FINAL;
	dfa[S_TYPEIDENTIFIER]['*']=S_TYPEIDENTIFIER_FINAL;
	dfa[S_TYPEIDENTIFIER]['/']=S_TYPEIDENTIFIER_FINAL;

	dfa[S_TYPEIDENTIFIER][';']=S_TYPEIDENTIFIER_FINAL;
}

void init_single_line_comment(short** dfa){
	printf("creating states for single line comment \n");

	set_transitions_printable(dfa,S_SINGLE_LINE_COMMENT,S_SINGLE_LINE_COMMENT);

	dfa[S_SLASH]['/']=S_SINGLE_LINE_COMMENT;

	set_transitions_letters(dfa,S_SINGLE_LINE_COMMENT,S_SINGLE_LINE_COMMENT);
	set_transitions_braces(dfa,S_SINGLE_LINE_COMMENT,S_SINGLE_LINE_COMMENT);
	set_transitions_digits(dfa,S_SINGLE_LINE_COMMENT,S_SINGLE_LINE_COMMENT);

	dfa[S_SINGLE_LINE_COMMENT][' ']=S_SINGLE_LINE_COMMENT;
	dfa[S_SINGLE_LINE_COMMENT]['\t']=S_SINGLE_LINE_COMMENT;

	//only a newline can get us out of this state

	dfa[S_SINGLE_LINE_COMMENT]['\n']=S_SINGLE_LINE_COMMENT_FINAL;
}


void init_multi_line_comment(short** dfa){
	printf("creating states for multi line comment \n");
	
	set_transitions_printable(dfa,S_MULTI_LINE_COMMENT,S_MULTI_LINE_COMMENT);

	set_transitions_breaking(dfa,S_MULTI_LINE_COMMENT,S_MULTI_LINE_COMMENT);

	set_transitions_letters(dfa,S_MULTI_LINE_COMMENT,S_MULTI_LINE_COMMENT);
	set_transitions_braces(dfa,S_MULTI_LINE_COMMENT,S_MULTI_LINE_COMMENT);
	set_transitions_digits(dfa,S_MULTI_LINE_COMMENT,S_MULTI_LINE_COMMENT);

	dfa[S_SLASH]['*']=S_MULTI_LINE_COMMENT;
	dfa[S_MULTI_LINE_COMMENT]['*']=S_MULTI_LINE_COMMENT_STAR;
	dfa[S_MULTI_LINE_COMMENT_STAR]['/']=S_MULTI_LINE_COMMENT_FINAL;

	dfa[S_MULTI_LINE_COMMENT][' ']=S_MULTI_LINE_COMMENT;
	dfa[S_MULTI_LINE_COMMENT]['\t']=S_MULTI_LINE_COMMENT;
	dfa[S_MULTI_LINE_COMMENT]['\n']=S_MULTI_LINE_COMMENT;
}

void init_eq(short** dfa){
	printf("creating states for '=','==' \n");
	dfa[S_EQ]['=']=S_EQ_FINAL;

	set_transitions_letters(dfa,S_EQ,S_EQ_FINAL);
	set_transitions_braces(dfa,S_EQ,S_EQ_FINAL);
	set_transitions_digits(dfa,S_EQ,S_EQ_FINAL);

	dfa[S_EQ][' ']=S_EQ_FINAL;

	dfa[S_EQ]['\'']=S_EQ_FINAL;
}

void init_numbers(short** dfa){
	printf("creating states for intgerers and floats \n");

	dfa[S_DIGITS]['.']=S_FLOAT;

	dfa[S_DIGITS][';']=S_INTEGER_FINAL;
	dfa[S_FLOAT][';']=S_FLOAT_FINAL;

	dfa[S_DIGITS][' ']=S_INTEGER_FINAL;
	dfa[S_FLOAT][' ']=S_FLOAT_FINAL;

	set_transitions_letters(dfa,S_DIGITS,S_INTEGER_FINAL);
	set_transitions_digits(dfa,S_DIGITS,S_DIGITS);

	set_transitions_letters(dfa,S_FLOAT,S_FLOAT_FINAL);
	set_transitions_digits(dfa,S_FLOAT,S_FLOAT);

	set_transitions_breaking(dfa,S_DIGITS,S_INTEGER_FINAL);
	set_transitions_breaking(dfa,S_DIGITS,S_INTEGER_FINAL);

	set_transitions_breaking(dfa,S_FLOAT,S_FLOAT_FINAL);
	set_transitions_breaking(dfa,S_FLOAT,S_FLOAT_FINAL);
}

void init_operator(short** dfa){
	printf("creating states for operators \n");
	dfa[S_START]['+']=S_OPERATOR_FINAL;
	dfa[S_START]['-']=S_MINUS;
	dfa[S_START]['*']=S_OPERATOR_FINAL;
	dfa[S_START]['/']=S_SLASH;
	dfa[S_START]['%']=S_OPERATOR_FINAL;
	dfa[S_START]['^']=S_OPERATOR_FINAL;
	dfa[S_START]['!']=S_NOT;

	dfa[S_NOT]['=']=S_OPERATOR_FINAL;

	dfa[S_START]['&']=S_OPERATOR_AND_1;
	dfa[S_OPERATOR_AND_1]['&']=S_OPERATOR_FINAL;

	dfa[S_START]['|']=S_OPERATOR_OR_1;
	dfa[S_OPERATOR_OR_1]['|']=S_OPERATOR_FINAL;

	set_transitions_letters(dfa,S_OPERATOR_AND_1,S_OPERATOR_FINAL);
	set_transitions_braces(dfa,S_OPERATOR_AND_1,S_OPERATOR_FINAL);


	set_transitions_letters(dfa,S_OPERATOR_OR_1,S_OPERATOR_FINAL);
	set_transitions_braces(dfa,S_OPERATOR_OR_1,S_OPERATOR_FINAL);

	set_transitions_letters(dfa,S_OPERATOR,S_OPERATOR_FINAL);
	set_transitions_braces(dfa,S_OPERATOR,S_OPERATOR_FINAL);

	dfa[S_OPERATOR][' ']=S_OPERATOR_FINAL;

	dfa[S_SLASH][' ']=S_OPERATOR_FINAL;
	set_transitions_letters(dfa,S_SLASH,S_OPERATOR_FINAL);
	set_transitions_braces(dfa,S_SLASH,S_OPERATOR_FINAL);
	set_transitions_digits(dfa,S_SLASH,S_OPERATOR_FINAL);
}

void init_return(short** dfa){
	printf("creating states for 'return' \n");
	//all other chars should get us back to S_IDENTIFIER,
	//to parse an identifier instead

	set_transitions_letters(dfa,S_R,S_IDENTIFIER);
	set_transitions_letters(dfa,S_RE,S_IDENTIFIER);
	set_transitions_letters(dfa,S_RET,S_IDENTIFIER);
	set_transitions_letters(dfa,S_RETU,S_IDENTIFIER);
	set_transitions_letters(dfa,S_RETUR,S_IDENTIFIER);
	set_transitions_letters(dfa,S_RETURN,S_IDENTIFIER);

	set_transitions_breaking(dfa,S_R,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_RE,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_RET,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_RETU,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_RETUR,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_RETURN,S_IDENTIFIER_FINAL);

	dfa[S_R]['e']=S_RE;
	dfa[S_RE]['t']=S_RET;
	dfa[S_RET]['u']=S_RETU;
	dfa[S_RETU]['r']=S_RETUR;
	dfa[S_RETUR]['n']=S_RETURN;


	dfa[S_RETURN][' ']=S_RETURN_FINAL;
	dfa[S_RETURN][';']=S_RETURN_FINAL;
	set_transitions_braces(dfa,S_RETURN,S_RETURN_FINAL);
}

void init_loop(short** dfa){
	printf("creating states for 'loop' \n");
	//all other chars should get us back to S_IDENTIFIER,
	//to parse an identifier instead

	set_transitions_letters(dfa,S_l,S_IDENTIFIER);
	set_transitions_letters(dfa,S_lo,S_IDENTIFIER);
	set_transitions_letters(dfa,S_loo,S_IDENTIFIER);
	set_transitions_letters(dfa,S_loop,S_IDENTIFIER);

	set_transitions_breaking(dfa,S_l,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_lo,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_loo,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_loop,S_loop_FINAL);

	dfa[S_l]['o']=S_lo;
	dfa[S_lo]['o']=S_loo;
	dfa[S_loo]['p']=S_loop;

	set_transitions_braces(dfa,S_loop,S_loop_FINAL);
}

void init_charconst(short** dfa){
	printf("creating states for charconst \n");

	set_transitions_printable(dfa,S_CHARCONST_1,S_CHARCONST_READ);

	dfa[S_START]['\'']=S_CHARCONST_1;
	set_transitions_letters(dfa,S_CHARCONST_1,S_CHARCONST_READ);
	set_transitions_digits(dfa,S_CHARCONST_1,S_CHARCONST_READ);

	dfa[S_CHARCONST_1]['\\']=S_CHARCONST_ESC;

	dfa[S_CHARCONST_ESC]['n']=S_CHARCONST_READ;
	dfa[S_CHARCONST_ESC]['t']=S_CHARCONST_READ;
	dfa[S_CHARCONST_ESC]['0']=S_CHARCONST_READ;

	dfa[S_CHARCONST_READ]['\'']=S_CHARCONST_FINAL;
}

void init_boolconst(short** dfa){
	printf("creating states for boolconst \n");

	set_transitions_letters(dfa,S_t,S_IDENTIFIER);
	set_transitions_letters(dfa,S_tr,S_IDENTIFIER);
	set_transitions_letters(dfa,S_tru,S_IDENTIFIER);
	set_transitions_letters(dfa,S_true,S_IDENTIFIER);

	set_transitions_braces(dfa,S_t,S_IDENTIFIER_FINAL);
	set_transitions_braces(dfa,S_tr,S_IDENTIFIER_FINAL);
	set_transitions_braces(dfa,S_tru,S_IDENTIFIER_FINAL);
	set_transitions_braces(dfa,S_true,S_IDENTIFIER_FINAL);

	set_transitions_letters(dfa,S_f,S_IDENTIFIER);
	set_transitions_letters(dfa,S_fa,S_IDENTIFIER);
	set_transitions_letters(dfa,S_fal,S_IDENTIFIER);
	set_transitions_letters(dfa,S_fals,S_IDENTIFIER);
	set_transitions_letters(dfa,S_false,S_IDENTIFIER);

	set_transitions_braces(dfa,S_f,S_IDENTIFIER_FINAL);
	set_transitions_braces(dfa,S_fa,S_IDENTIFIER_FINAL);
	set_transitions_braces(dfa,S_fal,S_IDENTIFIER_FINAL);
	set_transitions_braces(dfa,S_fals,S_IDENTIFIER_FINAL);
	set_transitions_braces(dfa,S_false,S_IDENTIFIER_FINAL);

	set_transitions_breaking(dfa,S_f,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_fa,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_fal,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_fals,S_IDENTIFIER_FINAL);

	set_transitions_breaking(dfa,S_t,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_tr,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_tru,S_IDENTIFIER_FINAL);

	dfa[S_START]['t']=S_t;
	dfa[S_t]['r']=S_tr;
	dfa[S_tr]['u']=S_tru;
	dfa[S_tru]['e']=S_true;

	dfa[S_START]['f']=S_f;
	dfa[S_f]['a']=S_fa;
	dfa[S_fa]['l']=S_fal;
	dfa[S_fal]['s']=S_fals;
	dfa[S_fals]['e']=S_false;

	dfa[S_true][' ']=S_BOOLCONST_FINAL;
	dfa[S_true][';']=S_BOOLCONST_FINAL;

	dfa[S_false][' ']=S_BOOLCONST_FINAL;
	dfa[S_false][';']=S_BOOLCONST_FINAL;

	set_transitions_braces(dfa,S_true,S_BOOLCONST_FINAL);
	set_transitions_braces(dfa,S_false,S_BOOLCONST_FINAL);

	
}

void init_struct(short** dfa){

	printf("creating states for struct \n");

	set_transitions_letters(dfa,S_s,S_IDENTIFIER);
	set_transitions_letters(dfa,S_st,S_IDENTIFIER);
	set_transitions_letters(dfa,S_stru,S_IDENTIFIER);
	set_transitions_letters(dfa,S_struc,S_IDENTIFIER);
	set_transitions_letters(dfa,S_struct,S_IDENTIFIER);

	dfa[S_START]['s']=S_s;
	dfa[S_s]['t']=S_st;
	dfa[S_st]['r']=S_str;
	dfa[S_str]['u']=S_stru;
	dfa[S_stru]['c']=S_struc;
	dfa[S_struc]['t']=S_struct;

	dfa[S_struct][' ']=S_struct_FINAL;
	set_transitions_breaking(dfa,S_struct,S_struct_FINAL);

	set_transitions_braces(dfa,S_struct,S_BOOLCONST_FINAL);

	set_transitions_breaking(dfa,S_s,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_st,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_str,S_IDENTIFIER_FINAL);
	set_transitions_breaking(dfa,S_stru,S_IDENTIFIER_FINAL);

}


void init_stringconst(short** dfa){

	printf("creating states for stringconst \n");

	set_transitions_printable(dfa,S_STRING,S_STRING);

	dfa[S_START]['"']=S_STRING;

	dfa[S_STRING]['\n']=S_STRING;
	dfa[S_STRING]['\t']=S_STRING;
	dfa[S_STRING]['\'']=S_STRING;
	dfa[S_STRING]['~']=S_STRING;
	dfa[S_STRING]['=']=S_STRING;

	//TODO:
	//...

	set_transitions_letters(dfa,S_STRING,S_STRING);
	set_transitions_digits(dfa,S_STRING,S_STRING);
	set_transitions_breaking(dfa,S_STRING,S_STRING);

	dfa[S_STRING]['"']=S_STRING_FINAL;
}


void init_typeparam(short** dfa){
	printf("creating states for stringconst \n");

	dfa[S_START]['?']=S_TPARAM_1;	//read '?'
	dfa[S_TPARAM_1]['T']=S_TPARAM_2;	//read 'T'

	set_transitions_digits(dfa,S_TPARAM_2,S_TPARAM_FINAL);

}

void init_arrow(short** dfa){
	printf("creating states for arrow \n");

	set_transitions_letters(dfa,S_MINUS,S_MINUS_FINAL);
	set_transitions_digits(dfa,S_MINUS,S_MINUS_FINAL);
	set_transitions_operators(dfa,S_MINUS,S_MINUS_FINAL);

	set_transitions_letters(dfa,S_WAVE,S_WAVE_FINAL);
	set_transitions_digits(dfa,S_WAVE,S_WAVE_FINAL);
	set_transitions_operators(dfa,S_WAVE,S_WAVE_FINAL);

	dfa[S_WAVE][' ']=S_WAVE_FINAL;
	dfa[S_MINUS][' ']=S_MINUS_FINAL;

	dfa[S_WAVE]['>']=S_ARROW_FINAL;	
	dfa[S_MINUS]['>']=S_ARROW_FINAL;	
}


void init_fn(short** dfa){
	printf("creating states for 'fn' \n");

	set_transitions_letters(dfa,S_fn,S_IDENTIFIER);

	dfa[S_f]['n']=S_fn;

	set_transitions_breaking(dfa,S_fn,S_fn_FINAL);
}


void set_transitions_lowercase(short** dfa, int state, int state_result){
	for(char i='a';i<='z';i++){
		dfa[state][i]=state_result;
	}
}
void set_transitions_uppercase(short** dfa, int state, int state_result){
	for(char i='A';i<='Z' ; i++){
		dfa[state][i]=state_result;
	}
}


void set_transitions_letters(short** dfa, int state, int state_result){
	//debug
	//printf("debug: set_transitions_letters \n");
	set_transitions_lowercase(dfa,state,state_result);
	set_transitions_uppercase(dfa,state,state_result);
}

void set_transitions_digits(short** dfa, int state, int state_result){
	for(char i='0';i<='9' ; i++){
		dfa[state][i]=state_result;
	}
}

void set_transitions_braces(short** dfa, int state, int state_result){
	dfa[state]['(']=state_result;
	dfa[state][')']=state_result;

	dfa[state]['{']=state_result;
	dfa[state]['}']=state_result;

	dfa[state]['[']=state_result;
	dfa[state][']']=state_result;
}

void set_transitions_operators(short** dfa, int state, int state_result){
	dfa[state]['+']=state_result;
	dfa[state]['-']=state_result;

	dfa[state]['*']=state_result;
	dfa[state]['/']=state_result;

	dfa[state]['&']=state_result;
	dfa[state]['|']=state_result;

	dfa[state]['<']=state_result;
	dfa[state]['>']=state_result;

	dfa[state]['%']=state_result;
	dfa[state]['=']=state_result;
}

void set_transitions_breaking(short** dfa, int state, int state_result){
	//anything that might abort parsing of identifiers
	//and keywords
	
	set_transitions_braces(dfa,state,state_result);
	set_transitions_operators(dfa,state,state_result);
	dfa[state][';']=state_result;
	dfa[state][' ']=state_result;
	dfa[state][',']=state_result;

	dfa[state]['\n']=state_result;
}

void set_transitions_printable(short** dfa, int state, int state_result){
	for(char i=32;i<=126;i++){
		dfa[state][i] = state_result;
	}
}