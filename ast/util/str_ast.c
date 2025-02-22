#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "str_ast.h"

static void error(char* msg) {

	fprintf(stderr, "[AST][Error] %s\n", msg);
}

//---------------------------------------

char* str_identifier(struct Id* id) {

	char* res = malloc(sizeof(char) * (strlen(id->identifier) + 1));
	if (!res) {
		return NULL;
	}
	sprintf(res, "%s", id->identifier);
	return res;
}

char* str_range(struct Range* r) {

	char* s1 = str_expr(r->start);
	if (!s1) {
		return NULL;
	}
	char* s2 = str_expr(r->end);
	if (!s2) {
		free(s1);
		return NULL;
	}

	uint16_t l = strlen(s1) + strlen(s2) + 4 + 1;

	char* res = malloc(sizeof(char) * l);
	if (!res) {
		free(s1);
		free(s2);
		return NULL;
	}

	sprintf(res, "%s .. %s", s1, s2);

	free(s1);
	free(s2);

	return res;
}

char* str_stmt_block(struct StmtBlock* block) {

	uint16_t l = 0;

	for (uint16_t i = 0; i < block->count; i++) {

		char* s = str_stmt(block->stmts[i]);

		if (!s) {
			return NULL;
		}

		l += strlen(s) + 1;

		free(s);
	}

	l += 1 + 2 + 2;

	char* res = malloc(sizeof(char) * l);

	if (!res) {
		return NULL;
	}

	strcpy(res, "{\n");

	for (uint16_t i = 0; i < block->count; i++) {

		char* s = str_stmt(block->stmts[i]);

		if (!s) {
			free(res);
			return NULL;
		}

		strcat(res, s);
		strcat(res, "\n");

		free(s);
	}
	strcat(res, "}\n");
	return res;
}

char* str_const_value(struct ConstValue* cv) {

	char* res = malloc(sizeof(char) * 129);

	if (!res) {
		return NULL;
	}

	switch (cv->kind) {
		case 1: strcpy(res, cv->ptr.m1_bool_const ? "true" : "false"); break;
		case 2: sprintf(res, "%d", cv->ptr.m2_int_const); break;
		case 3: sprintf(res, "'%c'", cv->ptr.m3_char_const); break;
		case 5: sprintf(res, "0x%x", cv->ptr.m5_hex_const); break;
		case 6: {
			uint32_t value = cv->ptr.m5_hex_const;

			if (value == 0) {
				sprintf(res, "0b0");
				return res;
			}

			const int size = 128;

			char buffer[size];

			int index = size - 1;
			buffer[index--] = '\0';

			while (value > 0) {

				uint8_t bit = value & 0x1;

				buffer[index--] = (bit == 0x1) ? '1' : '0';

				value >>= 1;
			}

			sprintf(res, "0b%s", buffer + index + 1);
		} break;
		default:
			free(res);
			fprintf(stderr, "[str_ast][Error] Unhandled Case\n");
			return NULL;
	}

	return res;
}

char* str_string_const(struct StringConst* s) {

	char* res = malloc(sizeof(char) * (3 + strlen(s->value)));
	if (!res) {
		return NULL;
	}
	sprintf(res, "%s", s->value);
	return res;
}

char* str_array_type(struct ArrayType* at) {

	char* inner = str_type(at->element_type);

	if (!inner) {
		return NULL;
	}

	char* res = malloc(sizeof(char) * (strlen(inner) + 2 + 1));

	if (!res) {
		return NULL;
	}

	sprintf(res, "[%s]", inner);

	free(inner);

	return res;
}

char* str_basic_type(struct BasicType* btw) {

	if (btw->simple_type != NULL)
		return str_simple_type(btw->simple_type);

	if (btw->subr_type != NULL)
		return str_subr_type(btw->subr_type);

	error("strBasicTypeWrapped");
	return NULL;
}

char* str_simple_type(struct SimpleType* st) {

	if (st->primitive_type != NULL)
		return str_primitive_type(st->primitive_type);

	return str_struct_type(st->struct_type);
}

char* str_subr_type(struct SubrType* st) {

	// Step 1 : calculate size required

	size_t size = 0 + 1 + 4; //\0, ()->

	for (int i = 0; i < st->count_arg_types; i++) {

		char* ats = str_type(st->arg_types[i]);

		if (!ats) {
			return NULL;
		}

		size += strlen(ats) + 2; // ", "
		free(ats);
	}

	char* rts = str_type(st->return_type);

	if (!rts) {
		return NULL;
	}

	size += strlen(rts);
	free(rts);

	// Step 2 : generate string

	char* res = malloc(sizeof(char) * size);

	if (!res) {
		return NULL;
	}

	strcpy(res, "(");

	for (int i = 0; i < st->count_arg_types; i++) {

		char* argType = str_type(st->arg_types[i]);

		if (!argType) {
			free(res);
			return NULL;
		}

		strcat(res, argType);
		free(argType);

		if (i < (st->count_arg_types - 1))
			strcat(res, ", ");
	}

	strcat(res, (st->has_side_effects) ? ")~>" : ")->");

	char* returntype = str_type(st->return_type);

	if (!returntype) {
		free(res);
		return NULL;
	}

	strcat(res, returntype);
	free(returntype);

	return res;
}

char* str_type(struct Type* t) {

	if (t->basic_type != NULL) { return str_basic_type(t->basic_type); }

	if (t->type_param != NULL) { return str_type_param(t->type_param); }

	if (t->array_type != NULL) { return str_array_type(t->array_type); }

	error("str_type");
	return NULL;
}

char* str_type_param(struct TypeParam* t) {

	char* res = malloc(sizeof(char) * 4);
	if (!res) {
		return NULL;
	}
	strcpy(res, "?T");
	sprintf(res + 2, "%d", t->index);
	return res;
}

char* str_primitive_type(struct PrimitiveType* p) {

	char* res;

	if (p->is_int_type) {

		char* types[] =
		    {"int8", "int16", "int32", "int64",
		     "uint8", "uint16", "uint32", "uint64",
		     "int", "uint"};

		asprintf(&res, "%s", types[p->int_type]);
	}

	if (p->is_char_type) asprintf(&res, "%s", "char");
	if (p->is_bool_type) asprintf(&res, "%s", "bool");

	return res;
}

char* str_struct_type(struct StructType* s) {

	char* res;
	asprintf(&res, "%s", s->type_name);
	return res;
}

char* str_struct_decl(struct StructDecl* decl) {

	char* name = str_simple_type(decl->type);

	uint16_t l = strlen("struct   { } ") + strlen(name);

	char* memberStrs[decl->count_members];

	for (uint16_t i = 0; i < decl->count_members; i++) {

		char* s2 = str_struct_member(decl->members[i]);
		if (!s2) {
			return NULL;
		}
		l += strlen(s2);

		memberStrs[i] = s2;
	}

	char* res = malloc(sizeof(char) * l);

	if (!res) {
		return NULL;
	}

	sprintf(res, "struct %s {", decl->type->struct_type->type_name);

	for (uint16_t i = 0; i < decl->count_members; i++) {

		char* s = memberStrs[i];
		strcat(res, s);
		free(s);
	}

	strcat(res, "}");

	return res;
}

char* str_struct_member(struct StructMember* s) {

	char* s1 = str_type(s->type);

	if (!s1) {
		return NULL;
	}

	const int l = strlen(s1);

	char* res;

	asprintf(&res, "%s %s;", s1, s->name);

	return res;
}

char* str_variable(struct Variable* v) {

	char* s1 = str_simple_var(v->simple_var);

	if (!s1) {
		return NULL;
	}

	char* s2 = (v->member_access != NULL) ? str_variable(v->member_access) : "";

	if (!s2) {
		free(s1);
		return NULL;
	}

	uint16_t l = strlen(s1) + strlen(s2);

	char* res = malloc(sizeof(char) * (l + 1 + 1));

	if (!res) {
		return NULL;
	}

	strcpy(res, s1);

	if (v->member_access != NULL) {
		strcat(res, ".");
		strcat(res, s2);
	}

	free(s1);

	if (v->member_access != NULL) free(s2);

	return res;
}

char* str_simple_var(struct SimpleVar* s) {

	// Step 1 : calculate size required

	size_t size = strlen(s->name) + 1;

	for (int i = 0; i < s->count_indices; i++) {

		char* s2 = str_expr(s->indices[i]);
		size += strlen(s2) + 2; // "[]"
		free(s2);
	}

	// Step 2 : generate our string

	char* res = malloc(sizeof(char) * size);

	if (!res) {
		return NULL;
	}

	strcpy(res, s->name);

	for (uint16_t i = 0; i < s->count_indices; i++) {

		char* s1 = str_expr(s->indices[i]);

		if (!s1) {
			free(res);
			return NULL;
		}

		strcat(res, "[");
		strcat(res, s1);
		strcat(res, "]");

		free(s1);
	}

	return res;
}

char* str_expr(struct Expr* e) {

	char* strTerm1 = str_un_op_term(e->term1);

	if (!strTerm1) {
		return NULL;
	}

	char* strO = (e->op != OP_NONE) ? str_op(e->op) : "";

	if (!strO) {
		free(strTerm1);
		return NULL;
	}

	char* strTerm2 = (e->term2 != NULL) ? str_un_op_term(e->term2) : "";

	if (!strTerm2) {
		free(strTerm1);
		return NULL;
	}

	uint16_t l = strlen(strTerm1) + strlen(strO) + strlen(strTerm2) + 1;

	char* res = malloc(sizeof(char) * l);

	if (!res) {
		return NULL;
	}

	sprintf(res, "%s%s%s", strTerm1, strO, strTerm2);

	free(strTerm1);

	if (e->op != OP_NONE) { free(strO); }
	if (e->term2 != NULL) { free(strTerm2); }

	return res;
}

char* str_op(enum OP o) {

	char* res = malloc(sizeof(char) * 16);

	if (!res) {
		return NULL;
	}

	char* str;

	switch (o) {
		case OP_NONE: str = "ERR"; break;
		case OP_PLUS: str = "+"; break;
		case OP_MINUS: str = "-"; break;
		case OP_MULTIPLY: str = "*"; break;
		case OP_SHIFT_LEFT: str = "<<"; break;
		case OP_SHIFT_RIGHT: str = ">>"; break;
		case OP_AND: str = "&"; break;
		case OP_OR: str = "|"; break;
		case OP_XOR: str = "^"; break;

		//should not happen
		default: str = "fix str_op"; break;
	}

	sprintf(res, "%s", str);

	return res;
}

char* str_un_op_term(struct UnOpTerm* u) {

	char* strO = (u->op != OP_NONE) ? str_op(u->op) : "";

	if (!strO) {
		return NULL;
	}

	char* strT = str_term(u->term);

	if (!strT) {
		return NULL;
	}

	uint16_t l = strlen(strO) + strlen(strT) + 3;

	char* res = malloc(sizeof(char) * l);

	if (!res) {
		free(strT);
		return NULL;
	}

	sprintf(res, "%s%s", strO, strT);

	if (u->op != OP_NONE) { free(strO); }
	free(strT);

	return res;
}

char* str_term(struct Term* t) {

	switch (t->kind) {
		case 4: return str_call(t->ptr.m4);
		case 5: return str_expr(t->ptr.m5);
		case 6: return str_variable(t->ptr.m6);
		case 8: return str_string_const(t->ptr.m8);
		case 12: return str_const_value(t->ptr.m12);
		case 13: return str_mdirect(t->ptr.m13);
	}

	error("str_term");
	return NULL;
}

char* str_mdirect(struct MDirect* m) {

	char* se = str_expr(m->expr);

	if (!se) {
		return NULL;
	}

	char* res;
	asprintf(&res, "[%s, %d]", se, m->load_store_width);

	free(se);
	return res;
}

char* str_stmt(struct Stmt* stmt) {

	switch (stmt->kind) {

		case 1: return str_call(stmt->ptr.m1);
		case 2: return str_while_stmt(stmt->ptr.m2);
		case 3: return str_if_stmt(stmt->ptr.m3);
		case 4: return str_ret_stmt(stmt->ptr.m4);
		case 5: return str_assign_stmt(stmt->ptr.m5);
		case 7: return str_for_stmt(stmt->ptr.m7);
		case 9: return str_massign_stmt(stmt->ptr.m9);
		case 99: {
			//break,continue,throw,...
			char* res = malloc(sizeof(char) * 30);
			if (!res) {
				return NULL;
			}
			strcpy(res, "");

			if (stmt->is_break) { sprintf(res, "break"); }
			if (stmt->is_continue) { sprintf(res, "continue"); }
			return res;
		}

		default:
			error("str_stmt");
			return NULL;
	}
}

char* str_assign_stmt(struct AssignStmt* a) {

	char* strOptType = "";

	if (a->opt_type != NULL) {
		strOptType = str_type(a->opt_type);
		if (!strOptType) {
			return NULL;
		}
	}

	char* strVar = str_variable(a->var);

	if (!strVar) {
		return NULL;
	}

	char* strE = str_expr(a->expr);

	if (!strE) {
		free(strVar);
		return NULL;
	}

	uint16_t l = strlen(strOptType) + strlen(strVar) + strlen(strE) + 1 + 4;

	char* res = malloc(sizeof(char) * l);

	if (!res) {
		free(strVar);
		free(strE);
		return NULL;
	}

	sprintf(res, "%s %s = %s", strOptType, strVar, strE);

	if (a->opt_type != NULL) free(strOptType);

	free(strVar);
	free(strE);
	return res;
}

char* str_for_stmt(struct ForStmt* f) {

	char* s1 = str_range(f->range);

	if (!s1) {
		return NULL;
	}

	char* s2 = str_stmt_block(f->block);

	if (!s2) {
		free(s1);
		return NULL;
	}

	char* res;
	asprintf(&res, "for %s in %s %s", f->index_name, s1, s2);

	free(s1);
	free(s2);
	return res;
}

char* str_if_stmt(struct IfStmt* i) {

	char* s1 = str_expr(i->condition);

	if (!s1) {
		return NULL;
	}

	char* s2 = str_stmt_block(i->block);

	if (!s2) {
		free(s1);
		return NULL;
	}

	char* s3 = "";

	if (i->else_block != NULL) {
		s3 = str_stmt_block(i->else_block);

		if (!s3) {
			free(s1);
			free(s2);
			return NULL;
		}
	}

	const uint32_t l = strlen(s1) + strlen(s2) + strlen(s3) + 1 + 10;

	char* res = malloc(sizeof(char) * l);

	if (!res) {
		free(s1);
		free(s2);
		return NULL;
	}

	sprintf(res, "if %s %s", s1, s2);

	free(s1);
	free(s2);

	if (i->else_block != NULL) {

		strcat(res, " else ");
		strcat(res, s3);
		free(s3);
	}

	return res;
}

char* str_while_stmt(struct WhileStmt* w) {

	char* s1 = str_expr(w->condition);

	if (!s1) {
		return NULL;
	}

	char* s2 = str_stmt_block(w->block);

	if (!s2) {
		free(s1);
		return NULL;
	}

	const uint32_t l = strlen(s1) + strlen(s2) + 5 + 2 + 1;

	char* res = malloc(sizeof(char) * l);

	if (!res) {
		free(s1);
		free(s2);
		return NULL;
	}

	sprintf(res, "while %s %s", s1, s2);

	free(s1);
	free(s2);

	return res;
}

char* str_call(struct Call* m) {

	// Step 1 : calculate size required

	uint16_t size = 1 + 2; // \0, "()"

	for (int i = 0; i < m->count_args; i++) {

		char* s = str_expr(m->args[i]);
		if (!s) {
			return NULL;
		}
		size += strlen(s) + 1; //","
		free(s);
	}

	char* str_var = str_variable(m->callable);

	if (!str_var) {
		return NULL;
	}

	// Step 2 : generate our string

	char* res = malloc(size + strlen(str_var));

	if (!res) {
		free(str_var);
		return NULL;
	}

	strcpy(res, str_var);
	free(str_var);
	strcat(res, "(");

	for (uint16_t i = 0; i < m->count_args; i++) {

		char* s1 = str_expr(m->args[i]);

		if (!s1) {
			free(res);
			return NULL;
		}

		strcat(res, s1);

		if (i != (m->count_args - 1)) {
			strcat(res, ",");
		}

		free(s1);
	}

	strcat(res, ")");

	return res;
}

char* str_ret_stmt(struct RetStmt* r) {

	char* s = str_expr(r->return_value);

	if (!s) {
		return NULL;
	}

	uint16_t l = 10 + strlen(s);

	char* res = malloc(sizeof(char) * l);

	if (!res) {
		free(s);
		return NULL;
	}

	sprintf(res, "return %s;", s);

	free(s);

	return res;
}

char* str_massign_stmt(struct MAssignStmt* m) {

	char* s1 = str_mdirect(m->lhs);

	if (!s1) {
		return NULL;
	}

	char* s2 = str_expr(m->expr);

	if (!s2) {
		free(s1);
		return NULL;
	}

	char* res = malloc(strlen(s1) + strlen(s2) + 6);

	if (!res) {
		free(s1);
		free(s2);
		return NULL;
	}

	sprintf(res, "%s = %s;", s1, s2);

	free(s1);
	free(s2);

	return res;
}
