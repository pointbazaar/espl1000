#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "avr_code_gen/cg_avr.h"

#include "../cli/flags/flags.h"
#include "util/fill_tables.h"
#include "util/fileutils/fileutils.h"

#include "util/ctx.h"
#include "invoke/invoke.h"
#include "compiler.h"
#include "tables/symtable/symtable.h"

#include "typechecker/typecheck.h"
#include "tables/sst/sst_fill.h"
#include "analyzer/fn/fn_analyzer.h"
#include "analyzer/dead/dead_analyzer.h"
#include "analyzer/halts/halt_analyzer.h"
#include "analyzer/annotation/annotation_analyzer.h"

bool compile(struct Flags* flags) {

	if (flags_count_filenames(flags) == 0) {
		printf("[Error] expected atleast 1 filename\n");
		return false;
	}

	struct AST* ast = make(AST);
	ast->count_namespaces = flags_count_filenames(flags);
	ast->namespaces = exit_malloc(sizeof(struct Namespace*) * flags_count_filenames(flags));

	for (int i = 0; i < flags_count_filenames(flags); i++) {

		char* filename = flags_filenames(flags, i);

		int status = invoke_lexer(filename);

		if (WEXITSTATUS(status) != 0) {
			printf("[Error] lexer exited with nonzero exit code\n");
			return false;
		}

		struct Namespace* ns = invoke_parser(filename);

		if (ns == NULL) {
			printf("[Error] parser exited with nonzero exit code\n");
			return false;
		}

		ast->namespaces[i] = ns;
	}

	struct Ctx* ctx = ctx_ctor(flags, makeST(flags_debug(flags)));

	fill_tables(ast, ctx);

	struct TCError* errors = typecheck_ast(ast, ctx_tables(ctx), true);

	if (errors != NULL) {
		return false;
	}

	analyze_functions(ctx_tables(ctx), ast);
	analyze_dead_code(ctx_tables(ctx), ast);
	analyze_termination(ctx_tables(ctx));
	analyze_annotations(ctx_tables(ctx), ast);

	bool success = compile_and_write_avr(ast, ctx);

	free_ast(ast);

	if (!success) {
		return false;
	}

	int status = 0;

	char cmd[200];
	sprintf(cmd, "avra %s > /tmp/avra-stdout 2> /tmp/avra-stderr", flags_asm_filename(flags));
	status = system(cmd);

	if (WEXITSTATUS(status) != 0) {
		printf("[Error] error with avra, see /tmp/avra-stdout, /tmp/avra-stderr \n");
	}

	ctx_dtor(ctx);

	return WEXITSTATUS(status) == 0;
}
