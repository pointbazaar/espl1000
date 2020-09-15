//standard headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//user headers

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

void init_if(short** dfa){
	
	set_transitions_abort_keyword(dfa,S_I);
	
	set_transitions_letters(dfa,S_IF,S_IDENTIFIER);

	dfa[S_I]['f']=S_IF;
	
	set_transitions_breaking(dfa,S_IF,S_IF_FINAL);
}

void init_else(short** dfa){

	set_transitions_abort_keyword(dfa,S_e);
	set_transitions_abort_keyword(dfa,S_el);
	set_transitions_abort_keyword(dfa,S_els);
	
	set_transitions_letters(dfa,S_else,S_IDENTIFIER);

	dfa[S_e]['l']=S_el;
	dfa[S_el]['s']=S_els;
	dfa[S_els]['e']=S_else;
	
	set_transitions_breaking(dfa,S_else,S_else_FINAL);
}

void init_while(short** dfa){

	//all other chars should get us back to S_IDENTIFIER,
	//to parse an identifier instead

	set_transitions_abort_keyword(dfa,S_W);
	set_transitions_abort_keyword(dfa,S_WH);
	set_transitions_abort_keyword(dfa,S_WHI);
	set_transitions_abort_keyword(dfa,S_WHIL);
	
	set_transitions_letters(dfa,S_WHILE,S_IDENTIFIER);
	
	set_transitions_breaking(dfa,S_WHILE,S_WHILE_FINAL);

	dfa[S_W]['h']=S_WH;
	dfa[S_WH]['i']=S_WHI;
	dfa[S_WHI]['l']=S_WHIL;
	dfa[S_WHIL]['e']=S_WHILE;
	dfa[S_WHILE][' ']=S_WHILE_FINAL;
}

void init_identifier(short** dfa){
	
	set_transitions_letters(dfa,S_IDENTIFIER,S_IDENTIFIER);
	set_transitions_digits(dfa,S_IDENTIFIER,S_IDENTIFIER);

	dfa[S_IDENTIFIER]['_']=S_IDENTIFIER;

	dfa[S_IDENTIFIER]['=']=S_IDENTIFIER_FINAL;
	
	dfa[S_IDENTIFIER]['.']=S_IDENTIFIER_FINAL;

	set_transitions_breaking(dfa,S_IDENTIFIER,S_IDENTIFIER_FINAL);
}

void init_typeidentifier(short** dfa){
	
	set_transitions_letters(dfa,S_TYPEIDENTIFIER,S_TYPEIDENTIFIER);
	set_transitions_digits(dfa,S_TYPEIDENTIFIER,S_TYPEIDENTIFIER);

	dfa[S_TYPEIDENTIFIER]['_']=S_TYPEIDENTIFIER;
	
	dfa[S_TYPEIDENTIFIER]['=']=S_TYPEIDENTIFIER_FINAL;
	
	dfa[S_TYPEIDENTIFIER]['.']=S_TYPEIDENTIFIER_FINAL;
	
	set_transitions_breaking(dfa,S_TYPEIDENTIFIER,S_TYPEIDENTIFIER_FINAL);
}

void init_single_line_comment(short** dfa){

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
	
	dfa[S_EQ]['=']=S_EQ_FINAL;

	set_transitions_letters(dfa,S_EQ,S_EQ_FINAL);
	set_transitions_braces(dfa,S_EQ,S_EQ_FINAL);
	set_transitions_digits(dfa,S_EQ,S_EQ_FINAL);

	dfa[S_EQ][' ']=S_EQ_FINAL;

	dfa[S_EQ]['\'']=S_EQ_FINAL;
}

void init_numbers(short** dfa){
	
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
	
	dfa[S_START]['+']=S_OPERATOR_FINAL;
	dfa[S_START]['-']=S_MINUS;
	dfa[S_START]['*']=S_OPERATOR_FINAL;
	dfa[S_START]['/']=S_SLASH;
	dfa[S_START]['%']=S_OPERATOR_FINAL;
	dfa[S_START]['^']=S_OPERATOR_FINAL;
	dfa[S_START]['!']=S_NOT;

	dfa[S_NOT]['=']=S_OPERATOR_FINAL;

	//logical operators
	dfa[S_START]['&']=S_OPERATOR_AND_1;
	dfa[S_OPERATOR_AND_1]['&']=S_OPERATOR_FINAL;

	dfa[S_START]['|']=S_OPERATOR_OR_1;
	dfa[S_OPERATOR_OR_1]['|']=S_OPERATOR_FINAL;
	
	//bitwise operators --------------------------
	//bitwise and
	dfa[S_OPERATOR_AND_1][' ']=S_OPERATOR_FINAL_2;
	dfa[S_OPERATOR_AND_1]['-']=S_OPERATOR_FINAL_2;
	set_transitions_letters(dfa, S_OPERATOR_AND_1, S_OPERATOR_FINAL_2);
	set_transitions_digits(dfa, S_OPERATOR_AND_1, S_OPERATOR_FINAL_2);
	set_transitions_braces(dfa,S_OPERATOR_AND_1,S_OPERATOR_FINAL_2);
	//bitwise or
	dfa[S_OPERATOR_OR_1][' ']=S_OPERATOR_FINAL_2;
	dfa[S_OPERATOR_OR_1]['-']=S_OPERATOR_FINAL_2;
	set_transitions_letters(dfa, S_OPERATOR_OR_1, S_OPERATOR_FINAL_2);
	set_transitions_digits(dfa, S_OPERATOR_OR_1, S_OPERATOR_FINAL_2);
	set_transitions_braces(dfa,S_OPERATOR_OR_1,S_OPERATOR_FINAL_2);
	//---------------------------------

	set_transitions_letters(dfa,S_OPERATOR,S_OPERATOR_FINAL_2);
	set_transitions_braces(dfa,S_OPERATOR,S_OPERATOR_FINAL_2);

	dfa[S_OPERATOR][' ']=S_OPERATOR_FINAL_2;

	dfa[S_SLASH][' ']=S_OPERATOR_FINAL_2;
	set_transitions_letters(dfa,S_SLASH,S_OPERATOR_FINAL_2);
	set_transitions_braces(dfa,S_SLASH,S_OPERATOR_FINAL_2);
	set_transitions_digits(dfa,S_SLASH,S_OPERATOR_FINAL_2);
	
	dfa[S_NOT][' '] = S_OPERATOR_FINAL_2;
	set_transitions_letters(dfa, S_NOT, S_OPERATOR_FINAL_2);
}

void init_return(short** dfa){
	
	//all other chars should get us back to S_IDENTIFIER,
	//to parse an identifier instead

	set_transitions_abort_keyword(dfa,S_R);
	set_transitions_abort_keyword(dfa,S_RE);
	set_transitions_abort_keyword(dfa,S_RET);
	set_transitions_abort_keyword(dfa,S_RETU);
	set_transitions_abort_keyword(dfa,S_RETUR);
	
	set_transitions_letters(dfa,S_RETURN,S_IDENTIFIER);

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
	
	//all other chars should get us back to S_IDENTIFIER,
	//to parse an identifier instead

	set_transitions_abort_keyword(dfa,S_l);
	set_transitions_abort_keyword(dfa,S_lo);
	set_transitions_abort_keyword(dfa,S_loo);
	
	set_transitions_letters(dfa,S_loop,S_IDENTIFIER);

	set_transitions_breaking(dfa,S_loop,S_loop_FINAL);

	dfa[S_l]['o']=S_lo;
	dfa[S_lo]['o']=S_loo;
	dfa[S_loo]['p']=S_loop;

	set_transitions_braces(dfa,S_loop,S_loop_FINAL);
}

void init_charconst(short** dfa){
	
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
	
	set_transitions_abort_keyword(dfa,S_t);
	set_transitions_abort_keyword(dfa,S_tr);
	set_transitions_abort_keyword(dfa,S_tru);
	
	set_transitions_letters(dfa,S_true,S_IDENTIFIER);
	set_transitions_braces(dfa,S_true,S_BOOLCONST_FINAL);

	set_transitions_abort_keyword(dfa,S_f);
	set_transitions_abort_keyword(dfa,S_fa);
	set_transitions_abort_keyword(dfa,S_fal);
	set_transitions_abort_keyword(dfa,S_fals);
	
	set_transitions_letters(dfa,S_false,S_IDENTIFIER);
	set_transitions_braces(dfa,S_false,S_IDENTIFIER_FINAL);

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

	set_transitions_abort_keyword(dfa,S_s);
	set_transitions_abort_keyword(dfa,S_st);
	set_transitions_abort_keyword(dfa,S_stru);
	set_transitions_abort_keyword(dfa,S_struc);
	
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
}

void init_stringconst(short** dfa){

	set_transitions_printable(dfa,S_STRING,S_STRING);

	dfa[S_START]['"']=S_STRING;

	dfa[S_STRING]['\n']=S_STRING;
	dfa[S_STRING]['\t']=S_STRING;
	dfa[S_STRING]['\'']=S_STRING;
	dfa[S_STRING]['~']=S_STRING;
	dfa[S_STRING]['=']=S_STRING;

	//TODO: ...

	set_transitions_letters(dfa,S_STRING,S_STRING);
	set_transitions_digits(dfa,S_STRING,S_STRING);
	set_transitions_breaking(dfa,S_STRING,S_STRING);

	dfa[S_STRING]['"']=S_STRING_FINAL;
}


void init_typeparam(short** dfa){
	
	dfa[S_START]['?']=S_TPARAM_1;	//read '?'
	dfa[S_TPARAM_1]['T']=S_TPARAM_2;	//read 'T'

	set_transitions_digits(dfa,S_TPARAM_2,S_TPARAM_FINAL);
}

void init_arrow(short** dfa){
	
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
	
	set_transitions_letters(dfa,S_fn,S_IDENTIFIER);

	dfa[S_f]['n']=S_fn;

	set_transitions_breaking(dfa,S_fn,S_fn_FINAL);
}

void init_break(short** dfa){
	
	set_transitions_abort_keyword(dfa, S_b);
	dfa[S_b]['r'] = S_br;
	
	set_transitions_abort_keyword(dfa, S_br);
	dfa[S_br]['e'] = S_bre;
	
	set_transitions_abort_keyword(dfa, S_bre);
	dfa[S_bre]['a'] = S_brea;
	
	set_transitions_abort_keyword(dfa, S_brea);
	dfa[S_brea]['k'] = S_break;
	
	set_transitions_letters(dfa, S_break, S_IDENTIFIER);
	dfa[S_break][';'] = S_break_FINAL;
	dfa[S_break][' '] = S_break_FINAL;
}

void init_for(short** dfa){
	
	set_transitions_abort_keyword(dfa, S_fo);
	set_transitions_abort_keyword(dfa, S_for);
	
	dfa[S_f]['o'] = S_fo;
	dfa[S_fo]['r'] = S_for;
	
	set_transitions_breaking(dfa, S_for, S_for_FINAL);
}

void init_in(short** dfa){

	dfa[S_I]['n'] = S_in;
	set_transitions_breaking(dfa, S_in, S_in_FINAL);
}

void init_range_op(short** dfa){
	
	//also initializes the structmemberaccess OP
	//because it does not have it's own method
	set_transitions_breaking(dfa, S_dot, S_STRUCTMEMBERACCESS_FINAL);
	
	set_transitions_letters(dfa, S_dot, S_STRUCTMEMBERACCESS_FINAL);
	set_transitions_digits(dfa, S_dot, S_STRUCTMEMBERACCESS_FINAL);
	dfa[S_dot]['.'] = S_RANGE_OP_FINAL;
}

void init_switch(short** dfa){
	
	
	set_transitions_abort_keyword(dfa, S_sw);
	set_transitions_abort_keyword(dfa, S_swi);
	set_transitions_abort_keyword(dfa, S_swit);
	set_transitions_abort_keyword(dfa, S_switc);
	set_transitions_abort_keyword(dfa, S_switch);
	
	dfa[S_s]['w']=S_sw;
	dfa[S_sw]['i']=S_swi;
	dfa[S_swi]['t']=S_swit;
	dfa[S_swit]['c']=S_switc;
	dfa[S_switc]['h']=S_switch;
	dfa[S_switch][' ']=S_switch_FINAL;
}

void init_case(short** dfa){
	
	set_transitions_abort_keyword(dfa, S_c);
	set_transitions_abort_keyword(dfa, S_ca);
	set_transitions_abort_keyword(dfa, S_cas);
	set_transitions_abort_keyword(dfa, S_case);
	
	dfa[S_c]['a']=S_ca;
	dfa[S_ca]['s']=S_cas;
	dfa[S_cas]['e']=S_case;
	dfa[S_case][' ']=S_case_FINAL;
}

// ---------------------------------------------------------

void set_transitions_lowercase(short** dfa, int state, short state_result){
	for(char i='a';i<='z';i++){
		dfa[state][(short)i]=state_result;
	}
}
void set_transitions_uppercase(short** dfa, int state, short state_result){
	for(char i='A';i<='Z' ; i++){
		dfa[state][(short)i]=state_result;
	}
}

void set_transitions_letters(short** dfa, int state, int state_result){
	
	set_transitions_lowercase(dfa,state,state_result);
	set_transitions_uppercase(dfa,state,state_result);
}

void set_transitions_digits(short** dfa, int state, int state_result){
	for(char i='0';i<='9' ; i++){
		dfa[state][(short)i]=state_result;
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
	//anything that might abort parsing of:
	// identifiers, typeidentifiers, keywords and numbers
	//and put them into the final state
	
	set_transitions_braces(dfa,state,state_result);
	set_transitions_operators(dfa,state,state_result);
	
	//punctuation
	dfa[state][';']=state_result;
	dfa[state][',']=state_result;
	dfa[state][':']=state_result;
	
	//whitespace
	dfa[state][' ']=state_result;
	dfa[state]['\n']=state_result;
	dfa[state]['\t']=state_result;
	
	//carriage return
	dfa[state]['\r']=state_result; 
}

void set_transitions_printable(short** dfa, int state, int state_result){
	for(char i=32;i<=126;i++){
		dfa[state][(short)i] = state_result;
	}
}

void set_transitions_abort_keyword(short** dfa, int state){
	//sets all transitions that abort 'reading' of a keyword
	//and move into the appropriate states.
	
	//this is not appropriate for the state before the final 
	//state of the keyword.
	
	set_transitions_letters(dfa, state, S_IDENTIFIER);
	set_transitions_digits(dfa, state, S_IDENTIFIER);
	set_transitions_breaking(dfa, state, S_IDENTIFIER_FINAL);
	
	//not breaking for numbers, therefore separate
	dfa[state]['.'] = S_IDENTIFIER_FINAL;
}
