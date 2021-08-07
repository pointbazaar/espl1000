#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ast/ast.h"
#include "ast/sd_inttype.h"

#include "flags.h"

static uint8_t sizeof_structmember(struct StructMember* m){
	//returns the size in bytes of that struct member
	//(assumes a 64 bit target machine)
	
	struct Type* type = m->type;
	
	if(type->m1 != NULL){
		struct BasicType* btw = type->m1;
		if(btw->simple_type != NULL){
			struct SimpleType* stype = btw->simple_type;
			if(stype->primitive_type != NULL){
				struct PrimitiveType* p = stype->primitive_type;
				
				if(p->is_float_type){ return 4; }
				if(p->is_char_type){ return 1; }
				if(p->is_bool_type){ return 1; }
				if(p->is_int_type){
					
					switch(p->int_type){
						case INT8:  case UINT8:  return 1;
						case INT16: case UINT16: return 2;
						case INT32: case UINT32: return 4;
						case INT64: case UINT64: return 8;
						case INT:   case UINT:   return 4; //assume
						case NONE: case INTTYPE_END: break;
					}
				}
			}
			if(stype->struct_type != NULL){
				return 8; //is pointer to struct
			}
		}
		if(btw->subr_type != NULL){ return 8;  }
	}
	
	if(type->m3 != NULL){
		return 8; //arraytype, is pointer
	}
	
	if(type->m2 != NULL){
	    //since it is a type param, it can be anything, so 8 bytes it is
        return 8;
	}

	printf("[smalldragon][codegen][Error] could not find size of struct member\n");
	exit(1);
}

static int compare_structmembers_size(const void* m1, const void* m2){
	
	uint8_t size1 = sizeof_structmember((struct StructMember*)m1);
	uint8_t size2 = sizeof_structmember((struct StructMember*)m2);
	
	return -(size1 - size2);
}

struct StructMember** struct_reorder(struct StructDecl* s){
	
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



