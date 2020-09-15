
#ifndef SS
#define SS
//S DEFINITIONS (afaik C has no const int)
//so we unfortunately have to use a macro

#define S_START 0

//TOKEN: if
#define S_I 1
#define S_IF 2
#define S_IF_FINAL 3

//TOKEN while
#define S_W 4
#define S_WH 5
#define S_WHI 6
#define S_WHIL 7
#define S_WHILE 8
#define S_WHILE_FINAL 9

//error states
#define S_ERROR 10

//end of file / end of string
#define S_EOS 11

#define S_NEWLINE_FINAL 12

//TOKEN identifier
#define S_IDENTIFIER 34
#define S_IDENTIFIER_FINAL 35


//TOKEN >
#define S_GREATER_FINAL 37

//TOKEN <
#define S_LESSER_FINAL 38


//TOKEN typeidentifier
#define S_TYPEIDENTIFIER 40
#define S_TYPEIDENTIFIER_FINAL 41


//COMMENTS
#define S_SINGLE_LINE_COMMENT 46
#define S_SINGLE_LINE_COMMENT_FINAL 47

#define S_MULTI_LINE_COMMENT 48
#define S_MULTI_LINE_COMMENT_STAR 49	//in a multiline comment, read a '*'
#define S_MULTI_LINE_COMMENT_FINAL 50

//TOKEN ;
#define S_SEMICOLON_FINAL 51

//TOKEN =
#define S_EQ 52
#define S_EQ_FINAL 53

//Numbers:
#define S_DIGITS 56	//is entered when we first see a digit

//TOKEN NONNEGATIVE_INTEGER
#define S_INTEGER_FINAL	57 //when we see a char thats not a digit and not '.' while being in state DIGIT or INTEGER

//TOKEN NONNEGATIVE_FLOAT
#define S_FLOAT 58 //when we have seen a '.' 
#define S_FLOAT_FINAL 59

//TOKEN {
#define S_LCURLY_FINAL 70
//TOKEN }
#define S_RCURLY_FINAL 71
//TOKEN (
#define S_LPARENS_FINAL 72
//TOKEN )
#define S_RPARENS_FINAL 73
//TOKEN [
#define S_LBRACKET_FINAL 74
//TOKEN ]
#define S_RBRACKET_FINAL 75

//TOKEN return
#define S_R 		76
#define S_RE 		77
#define S_RET 		78
#define S_RETU 		79
#define S_RETUR		80
#define S_RETURN	81
#define S_RETURN_FINAL 82

//TOKEN CHARCONST
#define S_CHARCONST_1 		83	//read "'"
#define S_CHARCONST_ESC		84

#define S_CHARCONST_READ  	88	//read a char
#define S_CHARCONST_FINAL 	89	//read final "'"

#define S_ANYTYPE_FINAL 90 // '#'

//TOKEN BOOLCONST
#define S_BOOLCONST_FINAL 91

#define S_t 92
#define S_tr 93
#define S_tru 94
#define S_true 95

#define S_f 96
#define S_fa 97
#define S_fal 98
#define S_fals 99
#define S_false 100

//TOKEN STRUCT
#define S_struct_FINAL 101

#define S_s 		102
#define S_st 		103
#define S_str 		104
#define S_stru 		105
#define S_struc 	106
#define S_struct 	107

//string constants
#define S_STRING 		108
#define S_STRING_FINAL 	109

//type parameter
#define S_TPARAM_1 110
#define S_TPARAM_2 111
#define S_TPARAM_FINAL 112

//TOKEN COMMA
#define S_COMMA_FINAL 114

//TOKEN ~
#define S_WAVE 			115
#define S_WAVE_FINAL 	116

//TOKEN ARROW
#define S_ARROW_FINAL	117

#define S_MINUS 		118
#define S_MINUS_FINAL 	119

//TOKEN LOOP
#define S_loop_FINAL 125

#define S_l 126
#define S_lo 127
#define S_loo 128
#define S_loop 129

//TOKEN OPERATOR
#define S_OPERATOR 			130
#define S_SLASH 			131	//'/'
#define S_OPERATOR_AND_1 	132	//'&'
#define S_OPERATOR_OR_1  	133	//'|'
#define S_NOT 				134	//'!'
//-----
#define S_OPERATOR_FINAL 	135 //no i--
//for when one token too much was read
#define S_OPERATOR_FINAL_2	136 //i--
//---------------------------

//TOKEN: else
#define S_e 			138
#define S_el 			139
#define S_els 			140
#define S_else 			141
#define S_else_FINAL 	142

//TOKEN: break
#define S_b				143
#define S_br			144
#define S_bre			145
#define S_brea			146
#define S_break			147
#define S_break_FINAL	148

//TOKEN: for
//S_f already exists
#define S_fo			149
#define S_for			150
#define S_for_FINAL		151

//TOKEN: in
//S_I already exists
#define S_in			152
#define S_in_FINAL		153

//'.'
#define S_dot			154
#define S_STRUCTMEMBERACCESS_FINAL 155

//'..'
#define S_RANGE_OP_FINAL	156

//TOKEN: switch
//S_s is already defined somewhere
#define S_sw			157
#define S_swi			158
#define S_swit			159
#define S_switc			160
#define S_switch		161
#define S_switch_FINAL	162

//TOKEN: case
#define S_c				163
#define S_ca			164
#define S_cas			165
#define S_case			166
#define S_case_FINAL	167

//TOKEN fn
#define S_fn 			168
#define S_fn_FINAL 		169

//so we set n_states to be sufficient, to
//have space for future states also

#endif
