//stdlib headers
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

//user headers
#include "tokens.h"
#include "lexer.h"
#include "strutils.h"
#include "states.h"
#include "init_dfa.h"


void init_dfa(short** dfa, bool* final_state, int n_states){

	bool debug = false;

	//for the identifiers
	set_transitions_lowercase(dfa,S_START,S_IDENTIFIER);

	//for the type identifiers
	set_transitions_uppercase(dfa,S_START,S_TYPEIDENTIFIER);

	//for the numbers
	set_transitions_digits(dfa,S_START,S_DIGITS);

	init_if(dfa);
	init_else(dfa);
	init_while(dfa);
	init_identifier(dfa);
	init_typeidentifier( dfa);
	init_single_line_comment(dfa);
	init_multi_line_comment( dfa);
	init_eq( dfa);
	init_numbers( dfa);
	init_operator(dfa);
	init_return(dfa);
	init_loop(dfa);
	init_charconst(dfa);
	init_boolconst(dfa);
	init_struct(dfa);
	init_stringconst(dfa);
	init_typeparam(dfa);

	init_arrow(dfa);

	init_fn(dfa);
	init_break(dfa);

	//SOME STARTING TRANSITIONS

	//whitespace between tokens should always be skipped
	dfa[S_START][' ']=S_START;
	//newlines between tokens should also be skipped
	dfa[S_START]['\n']=S_NEWLINE_FINAL;
	//tabs between tokens should be skipped
	dfa[S_START]['\t']=S_START;

	dfa[S_START]['b']=S_b;
	dfa[S_START]['i']=S_I;
	dfa[S_START]['w']=S_W;
	dfa[S_START]['r']=S_R;
	dfa[S_START]['l']=S_l;
	dfa[S_START]['e']=S_e;

	dfa[S_START]['{']=S_LCURLY_FINAL;
	dfa[S_START]['}']=S_RCURLY_FINAL;

	dfa[S_START]['(']=S_LPARENS_FINAL;
	dfa[S_START][')']=S_RPARENS_FINAL;

	dfa[S_START]['[']=S_LBRACKET_FINAL;
	dfa[S_START][']']=S_RBRACKET_FINAL;

	dfa[S_START]['<']=S_LESSER_FINAL;
	dfa[S_START]['>']=S_GREATER_FINAL;

	dfa[S_START]['~']=S_WAVE;
	dfa[S_START]['/']=S_SLASH;

	dfa[S_START][';']=S_SEMICOLON_FINAL;
	dfa[S_START]['=']=S_EQ;
	dfa[S_START]['#']=S_ANYTYPE_FINAL;

	dfa[S_START]['.']=S_STRUCTMEMBERACCESS_FINAL;
	dfa[S_START][',']=S_COMMA_FINAL;

	if(debug){
		printf("marking eos transitions\n");
	}
	//all states on encountering '\0' should go to eos state
	for(short i=0;i<n_states;i++){
		dfa[i]['\0']=S_EOS;
	}

	if(debug){
		printf("marking final states\n");
	}

	//set all states to be non-final states
	memset(final_state,false,n_states*sizeof(bool));

	final_state[S_IF_FINAL]=true;
	final_state[S_WHILE_FINAL]=true;
	final_state[S_IDENTIFIER_FINAL]=true;
	final_state[S_TYPEIDENTIFIER_FINAL]=true;

	final_state[S_LCURLY_FINAL]=true;
	final_state[S_RCURLY_FINAL]=true;

	final_state[S_LPARENS_FINAL]=true;
	final_state[S_RPARENS_FINAL]=true;

	final_state[S_LBRACKET_FINAL]=true;
	final_state[S_RBRACKET_FINAL]=true;

	final_state[S_LESSER_FINAL]=true;
	final_state[S_GREATER_FINAL]=true;
	final_state[S_WAVE_FINAL]=true;

	final_state[S_SINGLE_LINE_COMMENT_FINAL]=true;
	final_state[S_MULTI_LINE_COMMENT_FINAL]=true;

	final_state[S_SEMICOLON_FINAL]=true;
	final_state[S_EQ_FINAL]=true;

	final_state[S_INTEGER_FINAL]=true;
	final_state[S_FLOAT_FINAL]=true;

	final_state[S_OPERATOR_FINAL]=true;
	final_state[S_RETURN_FINAL]=true;
	final_state[S_loop_FINAL]=true;

	final_state[S_CHARCONST_FINAL]=true;
	final_state[S_ANYTYPE_FINAL]=true;
	final_state[S_BOOLCONST_FINAL]=true;
	final_state[S_struct_FINAL]=true;
	final_state[S_STRING_FINAL]=true;
	final_state[S_TPARAM_FINAL]=true;
	final_state[S_STRUCTMEMBERACCESS_FINAL]=true;
	final_state[S_COMMA_FINAL]=true;

	final_state[S_MINUS_FINAL]=true;
	final_state[S_ARROW_FINAL]=true;
	final_state[S_fn_FINAL]=true;
	final_state[S_else_FINAL]=true;

	final_state[S_NEWLINE_FINAL]=true;

	//error state should also be final. in error state we shall quit
	final_state[S_ERROR]=true;
	final_state[S_EOS]=true;

	//when we do not get into any state, 
	//that should reset start, so identifiers can be parsed correctly
	final_state[S_START]=true;
	
	final_state[S_break_FINAL]=true;
}
