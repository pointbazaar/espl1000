#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tables/lvst/lvst.h"
#include "tables/sst/sst.h"
#include "tac.h"

#include "_struct.h"

static char* get_op_str_expr(enum TAC_OP top) {
	char* opstr = "";
	switch (top) {
		default:
		case TAC_OP_NONE: opstr = "?ERR in get_op_str"; break;

		case TAC_OP_ADD: opstr = "+"; break;
		case TAC_OP_SUB: opstr = "-"; break;
		case TAC_OP_MUL: opstr = "*"; break;
		case TAC_OP_AND: opstr = "&"; break;
		case TAC_OP_OR: opstr = "|"; break;
		case TAC_OP_XOR: opstr = "^"; break;

		case TAC_OP_SHIFT_LEFT: opstr = "<<"; break;
		case TAC_OP_SHIFT_RIGHT: opstr = ">>"; break;

		case TAC_OP_CMP_LT: opstr = "<"; break;
		case TAC_OP_CMP_GE: opstr = ">="; break;
		case TAC_OP_CMP_EQ: opstr = "=="; break;
		case TAC_OP_CMP_NEQ: opstr = "!="; break;

		case TAC_OP_UNARY_NOT: opstr = "!"; break;
		case TAC_OP_UNARY_BITWISE_NEG: opstr = "~"; break;
		case TAC_OP_UNARY_MINUS: opstr = "-"; break;
	}
	return opstr;
}

static char* get_op_str(enum TAC_OP top) {
	char* opstr = "";
	switch (top) {
		default:
		case TAC_OP_NONE: opstr = "?ERR in get_op_str"; break;

		case TAC_OP_ADD: opstr = "+="; break;
		case TAC_OP_SUB: opstr = "-="; break;
		case TAC_OP_MUL: opstr = "*="; break;
		case TAC_OP_AND: opstr = "&="; break;
		case TAC_OP_OR: opstr = "|="; break;
		case TAC_OP_XOR: opstr = "^="; break;

		case TAC_OP_SHIFT_LEFT: opstr = "<<="; break;
		case TAC_OP_SHIFT_RIGHT: opstr = ">>="; break;

		case TAC_OP_CMP_LT: opstr = "<"; break;
		case TAC_OP_CMP_GE: opstr = ">="; break;
		case TAC_OP_CMP_EQ: opstr = "=="; break;
		case TAC_OP_CMP_NEQ: opstr = "!="; break;

		case TAC_OP_UNARY_NOT: opstr = "!"; break;
		case TAC_OP_UNARY_BITWISE_NEG: opstr = "~"; break;
		case TAC_OP_UNARY_MINUS: opstr = "-"; break;
	}
	return opstr;
}

char* tac_tostring(struct TAC* t, struct SST* sst, struct LVST* lvst) {

	char* res = malloc(sizeof(char) * 120);
	if (!res) {
		return NULL;
	}
	strcpy(res, "");

	char buf[110];
	strcpy(buf, "");

	const enum TAC_OP op = t->op;
	const uint32_t dest = t->dest;
	const uint64_t arg1 = t->arg1;
	const int64_t const_value = t->const_value;

	char* opstr = get_op_str(t->op);
	char* opstr_expr = get_op_str_expr(t->op);

	switch (t->kind) {
		case TAC_LABEL_INDEXED:
			sprintf(res, "L%d:", t->label_index);
			break;
		case TAC_LABEL_FUNCTION: {
			char* function_name = "main"; //in case sst is not initialized

			if (arg1 < sst_size(sst)) {
				function_name = sst_at(sst, dest)->name;
			}
			sprintf(res, "%-9s:", function_name);
		} break;

		case TAC_LOAD_FUNCTION_PTR: {
			char* name = sst_at(sst, arg1)->name;
			sprintf(buf, "t%d = %s", dest, name);
		} break;

		case TAC_GOTO:
			sprintf(buf, "goto L%d", t->label_index);
			break;
		case TAC_IF_GOTO:
			sprintf(buf, "if-goto t%lu L%d", arg1, t->label_index);
			break;
		case TAC_IF_CMP_GOTO:
			sprintf(buf, "if t%d %s t%lu goto L%d", dest, opstr_expr, arg1, t->label_index);
			break;

		case TAC_CONST_VALUE:
			sprintf(buf, "t%d = %ld", dest, const_value);
			break;

		case TAC_CONST_DATA:
			sprintf(buf, "t%d = %ld (DATA table offset)", dest, const_value);
			break;

		case TAC_COPY:
			sprintf(buf, "t%d = t%lu", dest, arg1);
			break;

		case TAC_LOAD_LOCAL_ADDR: {
			char* name = lvst_at(lvst, arg1)->name;
			const uint8_t width = tac_load_store_width(t);
			sprintf(buf, "load t%d = &l%lu (%s) (%d bytes)", dest, arg1, name, width);
		} break;

		case TAC_STORE_LOCAL: {
			char* name = lvst_at(lvst, dest)->name;
			sprintf(buf, "store l%d (%s) = t%lu", dest, name, arg1);
		} break;

		case TAC_LOAD: {
			const uint8_t width = tac_load_store_width(t);
			sprintf(buf, "t%d = [t%lu] (%d bytes)", dest, arg1, width);
			break;
		}
		case TAC_STORE: {
			const uint8_t width = tac_load_store_width(t);
			sprintf(buf, "[t%d] = t%lu (%d bytes)", dest, arg1, width);
			break;
		}

		case TAC_NOP:
			sprintf(buf, "%s", "nop");
			break;
		case TAC_BINARY_OP:
			if (op >= TAC_OP_CMP_LT && op <= TAC_OP_CMP_NEQ) {
				sprintf(buf, "t%d = t%d %s t%lu", dest, dest, opstr, arg1);
			} else {
				sprintf(buf, "t%d %s t%lu", dest, opstr, arg1);
			}
			break;
		case TAC_UNARY_OP:
			sprintf(buf, "t%d = %st%lu", dest, opstr, arg1);
			break;

		case TAC_CALL: {

			if (arg1 >= sst_size(sst)) {
				fprintf(stderr, "%s: function %lu not found\n", __func__, arg1);
				return NULL;
			}

			char* function_name = sst_at(sst, arg1)->name;
			sprintf(buf, "t%d = call %s", dest, function_name);
		} break;
		case TAC_ICALL: {
			sprintf(buf, "t%d = call t%ld", dest, arg1);
		} break;

		case TAC_PARAM:
			sprintf(buf, "param t%u (index %d)", dest, tac_param_index(t));
			break;
		case TAC_RETURN:
			sprintf(buf, "return t%d", dest);
			break;

		case TAC_SETUP_STACKFRAME:
			sprintf(buf, "setup_stackframe %ld", const_value);
			break;
		case TAC_SETUP_SP:
			sprintf(buf, "setup SP");
			break;
		default:
			fprintf(stderr, "unhandled in %s\n", __func__);
			return NULL;
			break;
	}

	strcat(res, buf);
	return res;
}

uint32_t tac_line_num(struct TAC* tac) {
	return tac->line_no;
}
