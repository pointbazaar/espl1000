#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "basicblock.h"
#include "ast/ast.h"
#include "tac/tacbuffer.h"
#include "basicblock_printer.h"

static void basicblock_dot_section(FILE* fout, uint32_t i, struct BasicBlock* block, struct Ctx* ctx);

void write_dot_file_from_graph(struct BasicBlock** blocks, uint32_t count, char* function_name, struct Ctx* ctx) {

	char fname_buffer[100];
	sprintf(fname_buffer, "%s.dot", function_name);
	FILE* fout = fopen(fname_buffer, "w");

	fprintf(fout, "digraph G {\n");

	//specify monospace font
	const char* str_font = "[fontname = \"monospace\"];\n";
	fprintf(fout, "graph %s node %s edge %s", str_font, str_font, str_font);

	for (uint32_t i = 0; i < count; i++) {
		struct BasicBlock* block = blocks[i];
		basicblock_dot_section(fout, i, block, ctx);
	}

	fprintf(fout, "}\n");

	fclose(fout);

	char cmd_buffer[100];

	sprintf(cmd_buffer, "dot -Tpng %s.dot > %s.png", function_name, function_name);
	system(cmd_buffer);
}

static void basicblock_dot_section(FILE* fout, uint32_t i, struct BasicBlock* block, struct Ctx* ctx) {

	char* tacbuffer_str = tacbuffer_tostring(block->buffer, true, ctx);

	fprintf(fout, "b%d [shape=box label=\"%s\"]\n", i, tacbuffer_str);

	free(tacbuffer_str);

	const uint8_t n_branches = (block->branch_1 == NULL && block->branch_2 == NULL) ? 0 : (block->branch_1 == NULL || block->branch_2 == NULL || block->branch_1 == block->branch_2) ? 1
	                                                                                                                                                                                 : 2;

	uint32_t target = 0;
	switch (n_branches) {
		case 0: break;

		case 1:
			//only 1 branch => unconditional

			if (block->branch_1 != NULL)
				target = block->branch_1->index;
			else if (block->branch_2 != NULL)
				target = block->branch_2->index;

			fprintf(fout, "b%d -> b%d\n", block->index, target);
			break;

		case 2:
			fprintf(fout, "b%d -> b%d [color=\"green\"]\n", block->index, block->branch_1->index);
			fprintf(fout, "b%d -> b%d [color=\"red\"]\n", block->index, block->branch_2->index);
			break;
	}
}
