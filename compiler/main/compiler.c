#include "avr_code_gen/cg_avr_single_function.h"
#include "x86_code_gen/cg_x86_single_function.h"
#define _GNU_SOURCE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include <unistd.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "avr_code_gen/cg_avr.h"
#include "lexer/src/lexer_flags.h"
#include "x86_code_gen/cg_x86.h"

#include "cli/flags/flags.h"
#include "util/fill_tables.h"
#include "util/fileutils/fileutils.h"

#include "util/ctx.h"
#include "compiler.h"
#include "tables/symtable/symtable.h"
#include "ibuffer/ibuffer_shared.h"

#include "typechecker/typecheck.h"
#include "tables/sst/sst_fill.h"
#include "analyzer/fn/fn_analyzer.h"
#include "analyzer/dead/dead_analyzer.h"
#include "analyzer/halts/halt_analyzer.h"
#include "analyzer/annotation/annotation_analyzer.h"
#include "analyzer/data/data_analyzer.h"

#include "lexer/src/lexer_main.h"
#include "parser/main/util/parser.h"

static bool call_assembler(struct Flags* flags) {

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

	if (WEXITSTATUS(system(cmd)) != 0) {

		fprintf(stderr, "error with %s, see %s, %s \n", prog, stdout_file, stderr_file);
		free(cmd);
		return false;
	}

	free(cmd);

	return true;
}

static bool compile_single_function(struct Ctx* ctx, struct Method* m, struct IBuffer* ibu) {

	struct Flags* flags = ctx_flags(ctx);

	if (flags_x86(flags)) {
		return compile_and_write_x86_single_function(m, ctx, ibu);
	} else {
		return compile_and_write_avr_single_function(m, ctx, ibu);
	}
}

static bool compile_common_loop(struct Ctx* ctx, struct AST* ast, struct IBuffer* ibu) {

	bool success = true;

	for (size_t i = 0; i < ast->count_namespaces; i++) {
		struct Namespace* ns = ast->namespaces[i];

		for (size_t j = 0; j < ns->count_methods; j++) {
			struct Method* m = ns->methods[j];

			success = compile_single_function(ctx, m, ibu);
			if (!success) {
				goto exit;
			}
		}
	}

exit:
	return success;
}

static bool compile_common(struct Ctx* ctx, struct AST* ast) {

	struct Flags* flags = ctx_flags(ctx);
	bool success = true;

	struct IBuffer* ibu = ibu_ctor();

	if (flags_x86(flags)) {
		if (!prologue_x86(ctx, ibu, ast)) {
			success = false;
			goto exit;
		}
	} else {
		//TODO: figure out how to support something like .data on AVR.
		if (data_count(ctx_tables(ctx)->data) != 0) {
			success = false;
			goto exit;
		}

		if (!avr_prologue(ctx, ibu)) {
			success = false;
			goto exit;
		}
	}

	compile_common_loop(ctx, ast, ibu);

	if (!ibu_write_to_file(ibu, flags_asm_filename(ctx_flags(ctx)))) {
		success = false;
		goto exit;
	}

exit:
	ibu_dtor(ibu);

	return success;
}

bool compile(struct Flags* flags) {

	const size_t count_filenames = flags_count_filenames(flags);

	if (count_filenames == 0) {
		printf("[Error] expected atleast 1 filename\n");
		return false;
	}

	struct AST* ast = make(AST);

	if (!ast) {
		return false;
	}

	ast->count_namespaces = flags_count_filenames(flags);
	ast->namespaces = malloc(sizeof(struct Namespace*) * count_filenames);

	if (!ast->namespaces) {
		free(ast);
		freeFlags(flags);
		return false;
	}

	struct TokenList** tokenLists = calloc(count_filenames, sizeof(struct TokenList*));

	assert(tokenLists);

	for (size_t i = 0; i < count_filenames; i++) {

		char* filename = flags_filenames(flags, i);

		struct LexerFlags lexer_flags;

		lexer_flags.write_token_file = flags_dump_tokens(flags);
		lexer_flags.filename = filename;
		lexer_flags.debug = flags_debug(flags);

		struct TokenList* list = lexer_main(&lexer_flags);

		if (!list) {
			fprintf(stderr, "[Error] lexer exited with nonzero exit code\n");
			free(ast->namespaces);
			free(ast);
			freeFlags(flags);
			return false;
		}

		tokenLists[i] = list;
	}

	if (flags_lexer(flags)) {
		if (flags_debug(flags)) {
			printf("[debug] exit after lexer\n");
		}
		return true;
	}

	for (size_t i = 0; i < count_filenames; i++) {

		char* filename = flags_filenames(flags, i);

		if (flags_debug(flags)) {
			printf("[debug] parsing %s\n", filename);
		}

		struct Namespace* ns = build_namespace(tokenLists[i], filename);

		if (ns == NULL) {
			fprintf(stderr, "[Error] parser exited with nonzero exit code\n");
			free(ast->namespaces);
			free(ast);
			freeFlags(flags);
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

	if (!fill_tables(ast, ctx)) {
		return false;
	}

	struct TCError* errors = typecheck_ast(ast, ctx, true);

	if (errors != NULL) {
		return false;
	}

	if (flags_debug(flags)) {
		printf("[debug] running analyzers...\n");
	}

	bool success;

	analyze_functions(ctx_tables(ctx), ast);
	success = analyze_dead_code(ctx, ast);

	if (!success) {
		return false;
	}

	analyze_termination(ctx_tables(ctx));
	analyze_annotations(ctx_tables(ctx), ast);
	analyze_data(ctx_tables(ctx), ast);

	if (flags_dump_data(flags)) {
		printf("\ndumping .data table:\n");

		struct IBuffer* dump = ibu_ctor();
		data_write_data_segment(ctx_tables(ctx)->data, dump);
		ibu_write(dump, stdout);
		ibu_dtor(dump);

		printf("\n");
	}

	success = compile_common(ctx, ast);

	free_ast(ast);

	if (!success) {
		return false;
	}

	for (size_t i = 0; i < count_filenames; i++) {
		freeTokenList(tokenLists[i]);
	}
	free(tokenLists);

	int status = 0;

	if (!call_assembler(flags)) {
		status = 1;
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
