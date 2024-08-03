#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "unicorn/unicorn.h"

#include "test_x86_code_gen_util.h"
#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_single_function.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"
#include "basic_block/basicblock.h"
#include "util/fill_tables.h"
#include "ibuffer/ibuffer.h"
#include "tables/symtable/symtable.h"

#include "../../cli/flags/flags.h"

uint64_t sd_uc_default_start_addr() {
	return 0x1000000; // old, unsure why this address
	//return 0x10000;
}

// callback for tracing instruction
static void hook_code(uc_engine* uc, uint64_t address, uint32_t size,
                      void* user_data) {
	uint64_t rip;

	uc_reg_read(uc, UC_X86_REG_RIP, &rip);
	printf(">>> Tracing instruction at 0x%" PRIx64 ", instruction size = 0x%x\n", address, size);
	//printf(">>> RIP is 0x%" PRIx64 "\n", rip);

	// Uncomment below code to stop the emulation using uc_emu_stop()
	// if (address == 0x1000009)
	//    uc_emu_stop(uc);
}

static void hook_mem64(uc_engine* uc, uc_mem_type type, uint64_t address,
                       int size, int64_t value, void* user_data) {
	switch (type) {
		default:
			printf(">>> %s hit default case with type=%d\n", __FUNCTION__, type);
			break;
		case UC_MEM_READ:
		case UC_MEM_READ_PROT:
		case UC_MEM_READ_UNMAPPED:
			printf(">>> Memory is being READ at 0x%" PRIx64 ", data size = %u\n", address, size);
			break;
		case UC_MEM_WRITE:
		case UC_MEM_WRITE_PROT:
		case UC_MEM_WRITE_UNMAPPED:
			printf(">>> Memory is being WRITE at 0x%" PRIx64 ", data size = %u, data value = 0x%" PRIx64 "\n", address, size, value);
			break;
	}
}

static void install_hooks(uc_engine* uc) {
	uc_err err;
	uc_hook trace1, trace2, trace3, trace4, trace5;
	// tracing all instruction by having @begin > @end
	err = uc_hook_add(uc, &trace1, UC_HOOK_CODE, hook_code, NULL, 1, 0);
	assert(err == UC_ERR_OK);

	// tracing all memory WRITE access (with @begin > @end)
	err = uc_hook_add(uc, &trace2, UC_HOOK_MEM_WRITE, hook_mem64, NULL, 1, 0);
	assert(err == UC_ERR_OK);
	err = uc_hook_add(uc, &trace3, UC_HOOK_MEM_WRITE_UNMAPPED, hook_mem64, NULL, 1, 0);
	assert(err == UC_ERR_OK);

	// tracing all memory READ access (with @begin > @end)
	err = uc_hook_add(uc, &trace4, UC_HOOK_MEM_READ, hook_mem64, NULL, 1, 0);
	assert(err == UC_ERR_OK);
	err = uc_hook_add(uc, &trace5, UC_HOOK_MEM_READ_UNMAPPED, hook_mem64, NULL, 1, 0);
	assert(err == UC_ERR_OK);
}

uc_err sd_uc_emu_start(struct sd_uc_engine* sd_uc, size_t nsteps, bool debug) {

	if (debug) {
		install_hooks(sd_uc->uc);
	}

	return uc_emu_start(sd_uc->uc, sd_uc->addr_start, sd_uc->addr_end, 0, nsteps);
}

void sd_uc_print_regs(struct sd_uc_engine* sduc) {

	uint64_t reg;
	char* names[] = {"rax", "rbx", "rcx", "rdx"};
	int indexes[] = {
	    UC_X86_REG_RAX,
	    UC_X86_REG_RBX,
	    UC_X86_REG_RCX,
	    UC_X86_REG_RDX,
	};
	for (int i = 0; i < sizeof(indexes) / sizeof(indexes[0]); i++) {
		uc_reg_read(sduc->uc, indexes[i], &reg);
		printf("%s = %04lx\n", names[i], reg);
	}
}

struct sd_uc_engine* sd_uc_engine_from_tacbuffer(struct TACBuffer* buffer) {

	//create the file
	FILE* falibi = fopen(".file.dg", "w");

	if (falibi == NULL) {
		fprintf(stderr, "error opening output file\n");
		exit(EXIT_FAILURE);
	}

	fclose(falibi);

	struct Flags* flags = makeFlagsSingleFile(".file.dg");

	struct Ctx* ctx = ctx_ctor(flags, st_ctor());

	FILE* fout = fopen(flags_asm_filename(flags), "w");

	if (fout == NULL) {
		fprintf(stderr, "error opening output file %s\n", flags_asm_filename(flags));
		exit(EXIT_FAILURE);
	}

	//fprintf(fout, "section .text\n");
	//fprintf(fout, "global _start\n");
	//fprintf(fout, "_start:\n");
	//
	//
	fprintf(fout, "BITS 64     ; specify 64-bit code\n");
	fprintf(fout, "start:\n");

	int nblocks;
	struct BasicBlock** graph = basicblock_create_graph(buffer, "main", &nblocks, ctx);

	struct BasicBlock* root = graph[0];

	if (root == NULL) {
		fprintf(stderr, "[Error] could not create BasicBlock.Exiting.\n");
		exit(EXIT_FAILURE);
	}

	struct IBuffer* ibu = ibu_ctor();

	emit_asm_x86_basic_block(root, ctx, ibu);

	for (int i = 0; i < nblocks; i++) {
		basicblock_dtor(graph[i]);
	}

	ibu_write(ibu, fout);

	free(graph);

	tacbuffer_dtor(buffer);

	ibu_dtor(ibu);

	fclose(fout);

	char cmd[200];
	//sprintf(cmd, "nasm -f elf64 %s -o file.o > /tmp/nasm-stdout 2> /tmp/nasm-stderr", flags_asm_filename(flags));
	sprintf(cmd, "nasm -f bin %s -o file.bin > /tmp/nasm-stdout 2> /tmp/nasm-stderr", flags_asm_filename(flags));

	int status = system(cmd);

	int status2 = WEXITSTATUS(status);

	if (WEXITSTATUS(status) != 0) {
		fprintf(stderr, "error with avra, see /tmp/nasm-stdout, /tmp/nasm-stderr \n");
		exit(EXIT_FAILURE);
	}

	/*
	status = system("ld -o file.bin file.o");
	if (WEXITSTATUS(status) != 0) {
	        printf("error with unable to link file.o\n");
	        exit(EXIT_FAILURE);
	}
	*/

	int fd = open("file.bin", O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	struct stat sb;
	if (fstat(fd, &sb) == -1) {
		perror("fstat");
		close(fd);
		exit(EXIT_FAILURE);
	}

	size_t filesize = sb.st_size;
	char* mapped = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
	if (mapped == MAP_FAILED) {
		perror("mmap");
		close(fd);
		exit(EXIT_FAILURE);
	}

	uc_engine* uc;
	uc_err err;

	err = uc_open(UC_ARCH_X86, UC_MODE_64, &uc);

	if (err != UC_ERR_OK) {
		fprintf(stderr, "Failed on uc_open() with error: %u\n", err);
		exit(EXIT_FAILURE);
	}

	struct sd_uc_engine* sd_uc = malloc(sizeof(struct sd_uc_engine));
	sd_uc->uc = uc;
	// memory address where emulation starts
	sd_uc->addr_start = sd_uc_default_start_addr();
	//sd_uc->addr_end = sd_uc->addr_start + filesize - 1;

	// stop on the address after the last instruction
	sd_uc->addr_end = sd_uc->addr_start + filesize;

	size_t mapped_size = 2 * 1024 * 1024; // old value from example
	//size_t mapped_size = 4 * 1024; // must be multiple of 4 KB

	if (sd_uc->addr_start + mapped_size <= sd_uc->addr_end) {
		assert(false);
	}
	{
		// expand the mapped size if needed.
		// apparently elf64-x86-64 files are a little heavy
		//mapped_size += 4 * 1024;
	}

	// like in the example code
	err = uc_mem_map(uc, sd_uc->addr_start, mapped_size, UC_PROT_ALL);
	if (err != UC_ERR_OK) {
		fprintf(stderr, "%s\n", uc_strerror(err));
		assert(false);
	}

	// write 'nop' to the part of the region for safety
	for (size_t i = sd_uc->addr_start; i < sd_uc->addr_start + 100; i++) {
		char* CODE_NOP = "\x90"; // nop
		err = uc_mem_write(uc, i, CODE_NOP, 1);
		if (err != UC_ERR_OK) {
			fprintf(stderr, "%s\n", uc_strerror(err));
			assert(false);
		}
	}

	// write machine code to memory
	if (uc_mem_write(uc, sd_uc->addr_start, mapped, filesize)) {
		fprintf(stderr, "Failed to write emulation code to memory\n");
		exit(EXIT_FAILURE);
	}

	ctx_dtor(ctx);

	return sd_uc;
}
