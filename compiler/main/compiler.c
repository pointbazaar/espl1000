#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "avr_code_gen/cg_avr.h"
#include "x86_code_gen/cg_x86.h"

#include "cli/flags/flags.h"
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

	const size_t count_filenames = flags_count_filenames(flags);

	if (count_filenames == 0) {
		printf("[Error] expected atleast 1 filename\n");
		return false;
	}

	struct AST* ast = make(AST);
	ast->count_namespaces = flags_count_filenames(flags);
	ast->namespaces = exit_malloc(sizeof(struct Namespace*) * count_filenames);

	for (int i = 0; i < count_filenames; i++) {

		char* filename = flags_filenames(flags, i);

		int status = invoke_lexer(filename);

		if (status != 0) {
			printf("[Error] lexer exited with nonzero exit code\n");
			return false;
		}
	}

	if (flags_lexer(flags)) {
		if (flags_debug(flags)) {
			printf("[debug] exit after lexer\n");
		}
		return true;
	}

	for (int i = 0; i < count_filenames; i++) {

		char* filename = flags_filenames(flags, i);

		struct Namespace* ns = invoke_parser(filename);

		if (ns == NULL) {
			printf("[Error] parser exited with nonzero exit code\n");
			return false;
		}

		ast->namespaces[i] = ns;
	}

	if (flags_parser(flags)) {
		if (flags_debug(flags)) {
			printf("[debug] exit after parser\n");
		}
		return true;
	}

	struct Ctx* ctx = ctx_ctor(flags, st_ctor());

	fill_tables(ast, ctx);

	struct TCError* errors = typecheck_ast(ast, ctx, true);

	if (errors != NULL) {
		return false;
	}

	if (flags_debug(flags)) {
		printf("[debug] running analyzers...\n");
	}

	analyze_functions(ctx_tables(ctx), ast);
	analyze_dead_code(ctx_tables(ctx), ast);
	analyze_termination(ctx_tables(ctx));
	analyze_annotations(ctx_tables(ctx), ast);

	bool success;
	if (flags_x86(flags)) {
		success = compile_and_write_x86(ast, ctx);
	} else {
		success = compile_and_write_avr(ast, ctx);
	}

	free_ast(ast);

	if (!success) {
		return false;
	}

	int status = 0;

	const char* prog;
	const char* stdout_file;
	const char* stderr_file;

	if (flags_x86(flags)) {
		prog = "nasm -felf64 -g";
		stdout_file = "/tmp/nasm-stdout";
		stderr_file = "/tmp/nasm-stdout";
	} else {
		prog = "avra";
		stdout_file = "/tmp/avra-stdout";
		stderr_file = "/tmp/avra-stdout";
	}

	const char* tmplt = "%s %s > %s 2> %s";
	char* cmd;
	asprintf(&cmd, tmplt, prog, flags_asm_filename(flags), stdout_file, stderr_file);

	status = system(cmd);

	free(cmd);

	if (WEXITSTATUS(status) != 0) {
		printf("[Error] error with %s, see %s, %s \n", prog, stdout_file, stderr_file);
		goto out;
	}

	if (flags_x86(flags) && !flags_nolink(flags)) {

		char* link_cmd = NULL;
		asprintf(&link_cmd, "ld -o %s %s", flags_output_filename(flags), flags_obj_filename(flags));
		if (flags_debug(flags)) {
			printf("%s\n", link_cmd);
		}
		status = system(link_cmd);

		free(link_cmd);

		if (WEXITSTATUS(status) != 0) {
			fprintf(stderr, "linking error\n");
			goto out;
		}
	}

out:
	ctx_dtor(ctx);

	return WEXITSTATUS(status) == 0;
}
