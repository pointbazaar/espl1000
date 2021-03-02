#ifndef MAGIC_NUM_H
#define MAGIC_NUM_H

//magic numbers for early detecting corrupt .ast files
//and for debugging .ast writer and reader

//additionally, the magic numbers of structures
//can be changend when their contents change,
//so old .ast files can be detected and re-generated

#define MAGIC_AST				32
#define MAGIC_NAMESPACE 		33

#define MAGIC_STRUCTDECL 		34
#define MAGIC_STMTBLOCK 		35
#define MAGIC_OP 				36
#define MAGIC_TERM 				37
#define MAGIC_EXPR 				38
#define MAGIC_UNOPTERM 			39
#define MAGIC_IDENTIFIER 		40
#define MAGIC_VARIABLE 			41
#define MAGIC_SIMPLEVAR 		42
#define MAGIC_RANGE 			43

#define MAGIC_BOOLCONST 		44
#define MAGIC_CHARCONST 		45
#define MAGIC_INTCONST 			46
#define MAGIC_HEXCONST 			47
#define MAGIC_BINCONST 			48
#define MAGIC_FLOATCONST 		49
#define MAGIC_STRINGCONST 		50

#define MAGIC_ASSIGNSTMT 		51
#define MAGIC_IFSTMT 			52
#define MAGIC_METHODCALL 		53
#define MAGIC_RETSTMT 			54
#define MAGIC_STMT 				55
#define MAGIC_WHILESTMT 		56
#define MAGIC_LOOPSTMT 			57
#define MAGIC_BREAKSTMT 		58
#define MAGIC_FORSTMT 			59
#define MAGIC_SWITCHSTMT 		60
#define MAGIC_CASESTMT 			61
#define MAGIC_TYPE 				62
#define MAGIC_ARRAYTYPE 		63
#define MAGIC_BASICTYPEWRAPPED 	64
#define MAGIC_SIMPLETYPE 		65
#define MAGIC_SUBRTYPE 			66
#define MAGIC_TYPEPARAM 		67
#define MAGIC_DECLARG 			68
#define MAGIC_STRUCTMEMBER 		69
#define MAGIC_METHOD 			70


//additionally, we define MAGIC_<STR>_END constants
//in order to assess if a structure was correctly deserialized
//(this verifies we did not read too much or too little from 
//the file, but just enough)


#define MAGIC_END_AST			200
#define MAGIC_END_NAMESPACE 	201

#define MAGIC_END_STRUCTDECL 	202
#define MAGIC_END_STMTBLOCK 	203
#define MAGIC_END_OP 			204
#define MAGIC_END_TERM 			205
#define MAGIC_END_EXPR 			206
#define MAGIC_END_UNOPTERM 		207
#define MAGIC_END_IDENTIFIER	208
#define MAGIC_END_VARIABLE 		209
#define MAGIC_END_SIMPLEVAR 	210
#define MAGIC_END_RANGE 		211

#define MAGIC_END_BOOLCONST 	212
#define MAGIC_END_CHARCONST 	213
#define MAGIC_END_INTCONST 		214
#define MAGIC_END_HEXCONST 		215
#define MAGIC_END_BINCONST 		216
#define MAGIC_END_FLOATCONST 	217
#define MAGIC_END_STRINGCONST 	218

#define MAGIC_END_ASSIGNSTMT 	219
#define MAGIC_END_IFSTMT 		220
#define MAGIC_END_METHODCALL 	221
#define MAGIC_END_RETSTMT 		222
#define MAGIC_END_STMT 			223
#define MAGIC_END_WHILESTMT 	224
#define MAGIC_END_LOOPSTMT 		225
#define MAGIC_END_BREAKSTMT 	226
#define MAGIC_END_FORSTMT 		227
#define MAGIC_END_SWITCHSTMT 	228
#define MAGIC_END_CASESTMT 		229
#define MAGIC_END_TYPE 			230
#define MAGIC_END_ARRAYTYPE 	231
#define MAGIC_END_BASICTYPEWRAPPED 232
#define MAGIC_END_SIMPLETYPE 	233
#define MAGIC_END_SUBRTYPE 		234
#define MAGIC_END_TYPEPARAM 	235
#define MAGIC_END_DECLARG 		236
#define MAGIC_END_STRUCTMEMBER 	237
#define MAGIC_END_METHOD 		238

#endif
