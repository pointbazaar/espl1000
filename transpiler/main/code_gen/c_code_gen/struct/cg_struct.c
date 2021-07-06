#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "ast/sd_inttype.h"

#include "util/ctx.h"
#include "flags/flags.h"

#include "cg_struct.h"

#include "code_gen/c_code_gen/types/cg_types.h"

#include "code_gen/util/indent.h"
#include "code_gen/types/gen_c_types.h"
#include "code_gen/structs/structs_code_gen.h"

static uint8_t sizeof_structmember(struct StructMember* m){
	//returns the size in bytes of that struct member
	//(assumes a 64 bit target machine) 
	//TODO: adjust for other target architecture?
	
	struct Type* type = m->type;
	
	if(type->m1 != NULL){
		struct BasicType* btw = type->m1;
		if(btw->simpleType != NULL){
			struct SimpleType* stype = btw->simpleType;
			if(stype->primitiveType != NULL){
				struct PrimitiveType* p = stype->primitiveType;
				
				if(p->isFloatType){ return 4; }
				if(p->isCharType){ return 1; }
				if(p->isBoolType){ return 1; }
				if(p->isIntType){
					
					switch(p->intType){
						case INT8:  case UINT8:  return 1;
						case INT16: case UINT16: return 2;
						case INT32: case UINT32: return 4;
						case INT64: case UINT64: return 8;
						case INT:   case UINT:   return 4; //assume
						case NONE: case INTTYPE_END: break;
					}
				}
			}
			if(stype->structType != NULL){
				return 8; //is pointer to struct
			}
		}
		if(btw->subrType != NULL){ return 8;  }
	}
	
	if(type->m3 != NULL){
		return 8; //arraytype, is pointer
	}
	
	if(type->m2 != NULL){ /*Fatal*/ }
	
	printf("Fatal(in sizeof_structmember)\n"); 
	exit(1);
}

static int compare_structmembers_size(const void* m1, const void* m2){
	
	uint8_t size1 = sizeof_structmember((struct StructMember*)m1);
	uint8_t size2 = sizeof_structmember((struct StructMember*)m2);
	
	return -(size1 - size2);
}

static struct StructMember** struct_reorder(struct StructDecl* s){
	
	//order struct members by size
	//http://www.catb.org/esr/structure-packing/
	// - pointers
	// - 8 bytes (int64, uint64)
	// - 4 bytes (int32, uint32, float)
	// - 2 bytes (int16, uint16)
	// - 1 byte  (bool, char, int8, uint8)
	
	struct StructMember** res = malloc(sizeof(struct StructMember*)*s->count_members);
	memcpy(res, s->members, sizeof(struct StructMember*)*s->count_members);
	
	//bubble sort
	for(uint16_t n = 0; n < s->count_members; n++){
		for(uint16_t i = 0; i < s->count_members-1; i++){
			
			struct StructMember* m1 = res[i];
			struct StructMember* m2 = res[i+1];
			
			int c = compare_structmembers_size(m1, m2);
			if(c > 0){
				res[i] = m2;
				res[i+1] = m1;
			}
		}
	}
	
	return res;
}

void transpileStructDecl(struct StructDecl* s, struct Ctx* ctx){
	
	fprintf(ctx->file ,"struct %s {\n", s->type->structType->typeName);
	
	struct StructMember** reordered = struct_reorder(s);
			
	for(uint32_t i = 0; i < s->count_members; i++){
		
		transpileStructMember(reordered[i], ctx);
	}
	
	free(reordered);
	
	fprintf(ctx->file, "};\n");
}

void transpileStructMember(struct StructMember* m, struct Ctx* ctx){
	
	fprintf(ctx->file, "\t");
	
	bool isSubrType = false;
	//is it a function pointer?
	if(m->type->m1 != NULL){
		if(m->type->m1->subrType != NULL){
			isSubrType = true;
			strncpy(
				ctx->currentFunctionPointerVarOrArgName,
				m->name,
				DEFAULT_STR_SIZE
			);
		}
	}
	
	transpileType(m->type, ctx);
	
	if(!isSubrType){
		//with C function pointers, the identifier of the 
		//function pointer is between the types
		//describing it
		fprintf(ctx->file, " %s", m->name);
	}
	
	fprintf(ctx->file, ";\n");
}
