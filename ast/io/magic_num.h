#ifndef MAGIC_NUM_H
#define MAGIC_NUM_H

//magic numbers for early detecting corrupt .ast files
//and for debugging .ast writer and reader

//additionally, the magic numbers of structures
//can be changend when their contents change,
//so old .ast files can be detected and re-generated

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


//const
#define MAGIC_BOOLCONST 		44
#define MAGIC_CHARCONST 		45
#define MAGIC_INTCONST 			46
#define MAGIC_HEXCONST 			47
#define MAGIC_BINCONST 			48
#define MAGIC_FLOATCONST 		49
#define MAGIC_CONSTVALUE		50
#define MAGIC_STRINGCONST 		51

//statements
#define MAGIC_ASSIGNSTMT 		52
#define MAGIC_IFSTMT 			53
#define MAGIC_CALL				54
#define MAGIC_RETSTMT 			55
#define MAGIC_STMT 				56
#define MAGIC_WHILESTMT 		57
#define MAGIC_FORSTMT 			59
#define MAGIC_SWITCHSTMT 		60
#define MAGIC_CASESTMT 			61
#define MAGIC_MASSIGNSTMT		62

//c_types_util
#define MAGIC_TYPE 				63
#define MAGIC_ARRAYTYPE 		64
#define MAGIC_BASICTYPE      	65
#define MAGIC_SIMPLETYPE 		66
#define MAGIC_SUBRTYPE 			67
#define MAGIC_TYPEPARAM 		68
#define MAGIC_STRUCTTYPE		69
#define MAGIC_PRIMITIVETYPE		70


#define MAGIC_DECLARG 			80
#define MAGIC_STRUCTMEMBER 		81
#define MAGIC_METHOD 			82
#define MAGIC_LAMBDA			83
#define MAGIC_METHOD_DECL		84
#define MAGIC_EXTERNC			85

#define MAGIC_MDIRECT           86


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

//const
#define MAGIC_END_BOOLCONST 	212
#define MAGIC_END_CHARCONST 	213
#define MAGIC_END_INTCONST 		214
#define MAGIC_END_HEXCONST 		215
#define MAGIC_END_BINCONST 		216
#define MAGIC_END_FLOATCONST 	217
#define MAGIC_END_CONSTVALUE	218
#define MAGIC_END_STRINGCONST 	219

//statements
#define MAGIC_END_ASSIGNSTMT 	220
#define MAGIC_END_IFSTMT 		221
#define MAGIC_END_CALL			222
#define MAGIC_END_RETSTMT 		223
#define MAGIC_END_STMT 			224
#define MAGIC_END_WHILESTMT 	225
#define MAGIC_END_FORSTMT 		227
#define MAGIC_END_SWITCHSTMT 	228
#define MAGIC_END_CASESTMT 		229
#define MAGIC_END_MASSIGNSTMT	230

//c_types_util
#define MAGIC_END_TYPE 			240
#define MAGIC_END_ARRAYTYPE 	241
#define MAGIC_END_BASICTYPE     242
#define MAGIC_END_SIMPLETYPE 	243
#define MAGIC_END_SUBRTYPE 		244
#define MAGIC_END_TYPEPARAM 	245
#define MAGIC_END_STRUCTTYPE	246
#define MAGIC_END_PRIMITIVETYPE	247

#define MAGIC_END_DECLARG 		248
#define MAGIC_END_STRUCTMEMBER 	249
#define MAGIC_END_METHOD 		250
#define MAGIC_END_LAMBDA		251
#define MAGIC_END_METHOD_DECL	252
#define MAGIC_END_EXTERNC		253

#define MAGIC_END_MDIRECT       254

#endif
