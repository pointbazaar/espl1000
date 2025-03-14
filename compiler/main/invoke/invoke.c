#define _XOPEN_SOURCE

#include <stdbool.h>
#define _GNU_SOURCE 1
#include <libgen.h>

int asprintf(char** restrict strp, const char* restrict fmt, ...);

#include "invoke.h"
#include "parser/main/util/parser.h"

struct Namespace* invoke_parser(struct TokenList* list, char* filename_display) {

	struct Namespace* ns = build_namespace(list, filename_display);

	return ns;
}
