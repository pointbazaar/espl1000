#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "lexer.h"
#include "driver.h"

#include "../../token/TokenKeys.h"

uint32_t line_no = 1;

static int h2out(const char* buffer, char* str, int token, struct TokenList* list) {
	if (strncmp(buffer, str, strlen(str)) == 0) {
		out(list, token, str);
		return strlen(str);
	}
	return 0;
}

static int h2out_nostr(const char* buffer, char* str, int token, struct TokenList* list) {
	if (strncmp(buffer, str, strlen(str)) == 0) {
		out_nostr(list, token);
		return strlen(str);
	}
	return 0;
}

static int h2out_char(const char* buffer, char c, int token, struct TokenList* list) {
	if (buffer[0] == c) {
		out_nostr(list, token);
		return 1;
	}
	return 0;
}

#define H2OUT(buf, str, token, fileout) \
	if ((n = h2out(buf, str, token, fileout)) > 0) { return n; }

#define H2OUT_NOSTR(buf, str, token, fileout) \
	if ((n = h2out_nostr(buf, str, token, fileout)) > 0) { return n; }

#define H2OUT_CHAR(buf, str, token, fileout) \
	if ((n = h2out_char(buf, str, token, fileout)) > 0) { return n; }

// find ID | TYPEID delimiter token like , / | { ...
static ssize_t h2_find_delim(const char* buf, size_t length) {
	size_t i = 0;
	for (; i < length; i++) {
		char c = buf[i];
		if (!isalnum(c) && c != '_') {
			return i;
		}
	}
	if (i > 0) {
		return i;
	}
	return -1;
}

static int handler2_intconst(const char* buf, struct TokenList* o, size_t nchars_remain) {

	int i = 0;
	while (isdigit(buf[i]) && i < nchars_remain) {
		i++;
	}
	out_length(o, INTEGER, (char*)buf, i);
	return i;
}

static int handler2_binconst(const char* buf, struct TokenList* o, size_t nchars_remain) {

	int i = 0;
	while ((buf[i + 2] == '0' || buf[i + 2] == '1') && i < nchars_remain) {
		i++;
	}
	out_length(o, BINCONST, (char*)buf, i + 2);
	return i + 2;
}

static int handler2_hexconst(const char* buf, struct TokenList* o, size_t nchars_remain) {

	int i = 0;
	while (isxdigit(buf[i + 2]) && i < nchars_remain) {
		i++;
	}
	out_length(o, HEXCONST, (char*)buf, i + 2);
	return i + 2;
}

static size_t stringconst_len(const char* buf, size_t nchars_remain) {

	int tmp_len = 1;
	char prev = ' ';
	while (tmp_len < nchars_remain) {
		if (buf[tmp_len] == '"' && prev != '\\') {
			break;
		}
		prev = buf[tmp_len];
		tmp_len++;
	}

	return tmp_len;
}

static int handler2_stringconst(const char* buf, struct TokenList* o, size_t nchars_remain) {

	const int tmp_len = stringconst_len(buf, nchars_remain);

	char* plain = calloc(tmp_len, sizeof(char));
	strncpy(plain, buf + 1, tmp_len - 1);

	size_t len = tmp_len - 1;
	char* plain_escaped = calloc(tmp_len, sizeof(char));

	int escaped_index = 0;
	for (int i = 0; i < len; i++) {
		char c = plain[i];
		if (c == '\\' && i + 1 < len) {
			char next = plain[i + 1];
			if (next == 'n') {
				plain_escaped[escaped_index] = '\n';
				i++;
			}
			if (next == '"') {
				plain_escaped[escaped_index] = '"';
				i++;
			}
		} else {
			plain_escaped[escaped_index] = c;
		}
		escaped_index++;
	}

	free(plain);

	out_length(o, STRINGCONST, plain_escaped, escaped_index);

	free(plain_escaped);

	return tmp_len + 1;
}

static int handler2_charconst(const char* buf, struct TokenList* o, size_t nchars_remain) {

	if (buf[2] == '\'') {
		out_length(o, CCONST, (char*)buf, 3);
		return 3;
	}
	if (buf[1] == '\\' && buf[2] == 'n') {
		out_length(o, CCONST, "'\n'", 3);
		return 4;
	}
	if (buf[1] == '\\' && buf[2] == 't') {
		out_length(o, CCONST, "'\t'", 3);
		return 4;
	}
	if (buf[1] == '\\' && buf[3] == '\'') {
		out_length(o, CCONST, (char*)buf, 4);
		return 4;
	}

	return -1;
}

static int handler2_multiline_comment(const char* buf, struct TokenList* o, size_t nchars_remain) {

	int i = 2;
	while ((strncmp(buf + i, "*/", 2) != 0) && i < nchars_remain) {
		i++;
	}
	return i + 2;
}

static int handler2_comment(const char* buf, struct TokenList* o, size_t nchars_remain) {

	int i = 0;
	while (buf[i + 2] != '\n' && (i + 2) < nchars_remain) {
		i++;
	}
	return i + 2;
}

static int handler2_newline(const char* buf, struct TokenList* o, size_t nchars_remain) {

	line_no++;
	return 1;
}

static int handler2_include_decl(const char* buf, struct TokenList* o, size_t nchars_remain) {

	int i = 10;
	while (buf[i] != '>' && i < nchars_remain) {
		i++;
	}
	out_length(o, INCLUDE_DECL, (char*)(buf + 10), i - 10);
	return i + 1;
}

static int handler2(const char* buf, struct TokenList* o, size_t nchars_remain) {

	int n;

	if (buf[0] == ' ' || buf[0] == '\t') {
		return 1;
	}

	if (buf[0] == '/') {
		if (strncmp(buf, "//", 2) == 0) {
			return handler2_comment(buf, o, nchars_remain);
		}
		if (strncmp(buf, "/*", 2) == 0) {
			return handler2_multiline_comment(buf, o, nchars_remain);
		}
	}

	if (buf[0] == '\n') {
		return handler2_newline(buf, o, nchars_remain);
	}

	if (strncmp(buf, "++", 2) == 0) {
		out_nostr(o, ASSIGNOP_PLUS);
		out(o, INTEGER, "1");
		return 2;
	}
	if (strncmp(buf, "--", 2) == 0) {
		out_nostr(o, ASSIGNOP_MINUS);
		out(o, INTEGER, "1");
		return 2;
	}

	H2OUT_CHAR(buf, '[', LBRACKET, o);
	H2OUT_CHAR(buf, ']', RBRACKET, o);
	H2OUT_CHAR(buf, '{', LCURLY, o);
	H2OUT_CHAR(buf, '}', RCURLY, o);
	H2OUT_CHAR(buf, '(', LPARENS, o);
	H2OUT_CHAR(buf, ')', RPARENS, o);

	H2OUT_CHAR(buf, ',', COMMA, o);
	H2OUT_NOSTR(buf, "..", RANGEOP, o);
	H2OUT_CHAR(buf, '.', STRUCTMEMBERACCESS, o);
	H2OUT_NOSTR(buf, "->", ARROW_NO_SIDE_EFFECT, o);
	H2OUT_NOSTR(buf, "~>", ARROW_SIDE_EFFECT, o);

	H2OUT_NOSTR(buf, "+=", ASSIGNOP_PLUS, o);
	H2OUT_NOSTR(buf, "-=", ASSIGNOP_MINUS, o);
	H2OUT_NOSTR(buf, "<<=", ASSIGNOP_SHIFT_LEFT, o);
	H2OUT_NOSTR(buf, ">>=", ASSIGNOP_SHIFT_RIGHT, o);
	H2OUT_NOSTR(buf, "&=", ASSIGNOP_BITWISE_AND, o);
	H2OUT_NOSTR(buf, "|=", ASSIGNOP_BITWISE_OR, o);

	H2OUT_CHAR(buf, '+', OPKEY_ARITHMETIC_PLUS, o);
	H2OUT_CHAR(buf, '-', OPKEY_ARITHMETIC_MINUS, o);
	H2OUT_CHAR(buf, '*', OPKEY_ARITHMETIC_MUL, o);
	H2OUT_CHAR(buf, '/', OPKEY_ARITHMETIC_DIV, o);
	H2OUT_CHAR(buf, '%', OPKEY_ARITHMETIC_MOD, o);

	H2OUT_NOSTR(buf, "!=", OPKEY_RELATIONAL_NEQ, o);
	H2OUT_NOSTR(buf, "==", OPKEY_RELATIONAL_EQ, o);
	H2OUT_NOSTR(buf, ">=", OPKEY_RELATIONAL_GE, o);
	H2OUT_NOSTR(buf, "<=", OPKEY_RELATIONAL_LE, o);

	H2OUT_NOSTR(buf, "=", ASSIGNOP_SIMPLE, o);

	H2OUT_NOSTR(buf, "&&", OPKEY_LOGICAL_AND, o);
	H2OUT_NOSTR(buf, "||", OPKEY_LOGICAL_OR, o);
	H2OUT_NOSTR(buf, "!", OPKEY_LOGICAL_NOT, o);

	H2OUT_NOSTR(buf, "|", OPKEY_BITWISE_OR, o);
	H2OUT_NOSTR(buf, "&", OPKEY_BITWISE_AND, o);
	H2OUT_NOSTR(buf, "^", OPKEY_BITWISE_XOR, o);
	H2OUT_NOSTR(buf, "~", OPKEY_BITWISE_NOT, o);
	H2OUT_NOSTR(buf, "<<", OPKEY_BITWISE_SHIFT_LEFT, o);
	H2OUT_NOSTR(buf, ">>", OPKEY_BITWISE_SHIFT_RIGHT, o);

	H2OUT_CHAR(buf, '<', OPKEY_RELATIONAL_LT, o);
	H2OUT_CHAR(buf, '>', OPKEY_RELATIONAL_GT, o);

	H2OUT_CHAR(buf, ';', SEMICOLON, o);

	if (buf[0] == '@') {
		H2OUT_NOSTR(buf, "@halts", ANNOT_HALTS, o);
		H2OUT_NOSTR(buf, "@private", ANNOT_HALTS, o);
		H2OUT_NOSTR(buf, "@public", ANNOT_PUBLIC, o);
		H2OUT_NOSTR(buf, "@deprecated", ANNOT_DEPRECATED, o);
		H2OUT_NOSTR(buf, "@extern", ANNOT_EXTERN, o);
		H2OUT_NOSTR(buf, "@syscall", ANNOT_SYSCALL, o);
	}
	if (buf[0] == '#') {
		if (strncmp(buf, "#include <", 10) == 0) {
			return handler2_include_decl(buf, o, nchars_remain);
		}
		H2OUT_CHAR(buf, '#', ANYTYPE, o);
	}
	if (buf[0] == '"') {
		return handler2_stringconst(buf, o, nchars_remain);
	}
	if (buf[0] == '\'') {
		return handler2_charconst(buf, o, nchars_remain);
	}
	if (buf[0] == '?') {
		out_length(o, TPARAM, (char*)(buf + 2), 1);
		return 3;
	}

	if (strncmp(buf, "int", 3) == 0) {
		H2OUT_NOSTR(buf, "int8", TYPEID_PRIMITIVE_INT8, o);
		H2OUT_NOSTR(buf, "int16", TYPEID_PRIMITIVE_INT16, o);
		H2OUT_NOSTR(buf, "int32", TYPEID_PRIMITIVE_INT32, o);
		H2OUT_NOSTR(buf, "int64", TYPEID_PRIMITIVE_INT64, o);
		H2OUT_NOSTR(buf, "int", TYPEID_PRIMITIVE_INT, o);
	}
	if (strncmp(buf, "uint", 3) == 0) {
		H2OUT_NOSTR(buf, "uint8", TYPEID_PRIMITIVE_UINT8, o);
		H2OUT_NOSTR(buf, "uint16", TYPEID_PRIMITIVE_UINT16, o);
		H2OUT_NOSTR(buf, "uint32", TYPEID_PRIMITIVE_UINT32, o);
		H2OUT_NOSTR(buf, "uint64", TYPEID_PRIMITIVE_UINT64, o);
		H2OUT_NOSTR(buf, "uint", TYPEID_PRIMITIVE_UINT, o);
	}
	H2OUT_NOSTR(buf, "char", TYPEID_PRIMITIVE_CHAR, o);
	H2OUT_NOSTR(buf, "bool", TYPEID_PRIMITIVE_BOOL, o);

	H2OUT_NOSTR(buf, "true", BCONST_TRUE, o);
	H2OUT_NOSTR(buf, "false", BCONST_FALSE, o);

	H2OUT_NOSTR(buf, "fn", FN, o);
	H2OUT_NOSTR(buf, "struct", STRUCT, o);

	H2OUT_NOSTR(buf, "return", RETURN, o);
	H2OUT_NOSTR(buf, "local", KEYWORD_LOCAL, o);
	H2OUT_NOSTR(buf, "if", IF, o);
	H2OUT_NOSTR(buf, "else", ELSE, o);
	H2OUT_NOSTR(buf, "while", WHILE, o);
	H2OUT_NOSTR(buf, "for", FOR, o);
	H2OUT_NOSTR(buf, "in", KEYWORD_IN, o);
	H2OUT_NOSTR(buf, "break", KEYWORD_BREAK, o);
	H2OUT_NOSTR(buf, "continue", KEYWORD_CONTINUE, o);
	H2OUT_NOSTR(buf, "enum", KEYWORD_ENUM, o);

	if (strncmp(buf, "0x", 2) == 0) {
		return handler2_hexconst(buf, o, nchars_remain);
	}
	if (strncmp(buf, "0b", 2) == 0) {
		return handler2_binconst(buf, o, nchars_remain);
	}
	if (isdigit(buf[0])) {
		return handler2_intconst(buf, o, nchars_remain);
	}

	ssize_t index = h2_find_delim(buf, nchars_remain);
	if (index > 0) {
		if (islower(buf[0])) {
			out_length(o, ID, (char*)buf, index);
			return index;
		} else if (isupper(buf[0])) {

			bool all_upper = true;
			for (int i = 0; i < index; i++) {
				if (islower(buf[i])) {
					all_upper = false;
				}
			}

			if (all_upper) {
				out_length(o, TOKEN_ENUM_VALUE, (char*)buf, index);
			} else {
				out_length(o, TYPEID, (char*)buf, index);
			}
			return index;
		}
	}

	return -1;
}

struct TokenList* lexer_impl(FILE* infile) {

	struct stat sb;
	const int fd = fileno(infile);
	bool debug = false;
	int status = 0;

	if (fstat(fd, &sb) < 0) {
		fprintf(stderr, "[Lexer] could not fstat input file\n");
		return NULL;
	}

	const size_t length = sb.st_size;
	const char* addr = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);

	if (addr == MAP_FAILED) {
		fprintf(stderr, "[Lexer] could not mmap input file\n");
	}

	if (debug) {
		fprintf(stderr, "[Lexer] mmapped input file at %p, length %ld bytes\n", addr, length);
	}

	struct TokenList* list = makeTokenList();

	if (!list) {
		return NULL;
	}

	line_no = 1;

	char c;
	for (size_t i = 0; i < length;) {
		size_t nchars_remain = length - i;
		int ntokens = handler2(addr + i, list, nchars_remain);
		if (ntokens <= 0) {
			fprintf(stderr, "[Lexer] could not scan token at index %ld\n", i);
			fprintf(stderr, "%c\n", addr[i]);
			status = 1;
			break;
		}
		i += ntokens;
	}

	munmap((void*)addr, length);

	return list;
}
