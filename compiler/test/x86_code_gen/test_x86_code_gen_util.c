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

struct sd_uc_engine {
	// wrapper struct

	// addresses of the emulated code
	uint64_t addr_start;
	uint64_t addr_end;

	// our stack area
	uint64_t addr_stack;
	uint64_t size_stack;

	uc_engine* uc;
};

uint64_t sd_uc_default_start_addr() {
	return 0x1000000; // old, unsure why this address
	//return 0x10000;
}

uint64_t sd_uc_default_stack_addr() {
	return 0x2000000;
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
		case UC_MEM_FETCH_UNMAPPED:
			printf(">>> Instruction is being FETCH at 0x%" PRIx64 ", data size = %u\n", address, size);
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
	uc_hook trace1, trace2, trace3, trace4, trace5, trace6;
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

	err = uc_hook_add(uc, &trace6, UC_HOOK_MEM_FETCH_UNMAPPED, hook_mem64, NULL, 1, 0);
	assert(err == UC_ERR_OK);
}

uc_err sd_uc_emu_start(struct sd_uc_engine* sd_uc, size_t nsteps, bool debug) {

	if (debug) {
		install_hooks(sd_uc->uc);
	}

	uc_err err = uc_emu_start(sd_uc->uc, sd_uc->addr_start, sd_uc->addr_end, 0, nsteps);

	if (err != UC_ERR_OK){
		fprintf(stderr, "Error: (running %s\n):\n", __FUNCTION__);
		fprintf(stderr, "%s\n", uc_strerror(err));
	}

	return err;
}

void sd_uc_close(struct sd_uc_engine* sduc) {
	uc_close(sduc->uc);
}

void sd_uc_print_stack(struct sd_uc_engine* sduc) {

	uc_err err;
	uint64_t rsp;
	uint64_t rbp;
	err = uc_reg_read(sduc->uc, UC_X86_REG_RSP, &rsp);
	assert(err == UC_ERR_OK);
	err = uc_reg_read(sduc->uc, UC_X86_REG_RBP, &rbp);
	assert(err == UC_ERR_OK);
	const size_t window = 8 * 5;
	uint64_t start = rsp-window;
	uint64_t last = rsp+window;

	printf("-- stack contents --\n");
	for (uint64_t addr = start; addr <= last; addr+=8){
		uint64_t read = 0;
		err = sd_uc_mem_read64(sduc, addr, &read);
		if (err != UC_ERR_OK){
			printf("[%08lx] error reading memory", addr);
			return;
		}
		printf("[%08lx] = %08lx", addr, read);

		if(addr == rsp) printf(" <- rsp");
		if(addr == rbp) printf(" <- rbp");

		printf("\n");
	}
}

void sd_uc_print_regs(struct sd_uc_engine* sduc) {

	uint64_t reg;
	char* names[] = {
		"rax",
		"rbx",
		"rcx",
		"rdx",
		"rdi",
		"rsi",
		"rsp",
		"rbp",
	};
	int indexes[] = {
	    UC_X86_REG_RAX,
	    UC_X86_REG_RBX,
	    UC_X86_REG_RCX,
	    UC_X86_REG_RDX,
	    UC_X86_REG_RDI,
	    UC_X86_REG_RSI,
	    UC_X86_REG_RSP,
	    UC_X86_REG_RBP,
	};
	for (int i = 0; i < sizeof(indexes) / sizeof(indexes[0]); i++) {
		uc_reg_read(sduc->uc, indexes[i], &reg);
		printf("%s = %04lx\n", names[i], reg);
	}
}
static void sd_uc_engine_setup_stack(struct sd_uc_engine* sduc) {

	uc_engine* uc = sduc->uc;
	uc_err err;

	sduc->addr_stack = sd_uc_default_stack_addr();
	sduc->size_stack = 4096;

	uint64_t start_map = sduc->addr_stack - sduc->size_stack;
	size_t size_map = sduc->size_stack * 2;

	// like in the example code
	err = uc_mem_map(uc, start_map, size_map, UC_PROT_READ | UC_PROT_WRITE);
	if (err != UC_ERR_OK) {
		fprintf(stderr, "%s\n", uc_strerror(err));
		assert(false);
	}

	uc_reg_write(uc, UC_X86_REG_RSP, &(sduc->addr_stack));
}

static struct sd_uc_engine* sd_uc_engine_from_mapped(char* mapped, size_t filesize) {

	uc_engine* uc;
	uc_err err;

	err = uc_open(UC_ARCH_X86, UC_MODE_64, &uc);

	if (err != UC_ERR_OK) {
		fprintf(stderr, "Failed on uc_open() with error: %u\n", err);
		exit(EXIT_FAILURE);
	}

	struct sd_uc_engine* sd_uc = exit_malloc(sizeof(struct sd_uc_engine));
	sd_uc->uc = uc;
	// memory address where emulation starts
	sd_uc->addr_start = sd_uc_default_start_addr();

	// stop on the address after the last instruction
	sd_uc->addr_end = sd_uc->addr_start + filesize;

	size_t mapped_size = 2 * 1024 * 1024; // old value from example
	//size_t mapped_size = 4 * 1024; // must be multiple of 4 KB

	if (sd_uc->addr_start + mapped_size <= sd_uc->addr_end) {
		assert(false);
	}

	sd_uc_engine_setup_stack(sd_uc);

	// put the end address on the stack so when 'ret' happens
	// we end up at the end of our code
	err = uc_mem_write(uc, sd_uc->addr_stack, &(sd_uc->addr_end), sizeof(uint64_t));
	assert(err == UC_ERR_OK);

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

	return sd_uc;
}

static void sd_uc_fake_lvst(struct Ctx* ctx){
	struct ST* st = ctx_tables(ctx);
	assert(st != NULL);

	struct LVST* lvst = st->lvst;
	assert(lvst != NULL);
	struct LVSTLine* line = malloc(sizeof(struct LVSTLine));
	assert(line != NULL);

	sprintf(line->name, "fake variable");
	struct Type* type = calloc(1, sizeof(struct Type));

	struct BasicType* basicType = calloc(1, sizeof(struct BasicType));
	type->basic_type = basicType;

	struct SimpleType* simpleType = calloc(1, sizeof(struct SimpleType));
	basicType->simple_type = simpleType;

	struct PrimitiveType* pt = calloc(1, sizeof(struct PrimitiveType));
	simpleType->primitive_type = pt;
	pt->is_int_type = true;
	pt->int_type = UINT64;

	line->type = type;
	line->first_occur = NULL;
	line->is_arg = false;
	line->read_only = false;

	lvst_add(lvst, line);
}

static struct sd_uc_engine* sd_uc_engine_from_tacbuffer_common(struct TACBuffer* buffer, bool debug, bool create_fake_lvst) {

	char* argv_debug[] = {"program", "-debug", ".file.dg"};
	char* argv_common[] = {"program", ".file.dg"};
	char** argv;
	int argc;
	if (debug){
		argv = argv_debug;
		argc = 3;
	} else {
		argv = argv_common;
		argc = 2;
	}
	struct Flags* flags = makeFlags(argc, argv);
	assert(flags != NULL);

	struct Ctx* ctx = ctx_ctor(flags, st_ctor());
	assert(ctx != NULL);

	if (create_fake_lvst){
		sd_uc_fake_lvst(ctx);
	}

	FILE* fout = fopen(flags_asm_filename(flags), "w");

	if (fout == NULL) {
		fprintf(stderr, "error opening output file %s\n", flags_asm_filename(flags));
		exit(EXIT_FAILURE);
	}

	// We use a flat binary without sections. So only the instructions.
	// Because that's easier for unicorn engine than a full elf64 binary.
	fprintf(fout, "BITS 64     ; specify 64-bit code\n");
	fprintf(fout, "start:\n");

	int nblocks;
	struct BasicBlock** graph = basicblock_create_graph(buffer, "main", &nblocks, ctx);
	assert(graph != NULL);

	struct BasicBlock* root = graph[0];

	if (root == NULL) {
		fprintf(stderr, "[Error] could not create BasicBlock.Exiting.\n");
		exit(EXIT_FAILURE);
	}

	struct IBuffer* ibu = ibu_ctor();

	struct RAT* rat = rat_ctor(RAT_ARCH_X86);

	for (int i = 0; i < nblocks; i++) {
		allocate_registers(graph[i]->buffer, rat, ctx_tables(ctx));
	}

	emit_asm_x86_basic_block(root, ctx, ibu, rat);

	for (int i = 0; i < nblocks; i++) {
		basicblock_dtor(graph[i]);
	}

	ibu_write(ibu, fout);

	rat_dtor(rat);

	free(graph);

	tacbuffer_dtor(buffer);

	ibu_dtor(ibu);

	fclose(fout);

	char cmd[200];
	sprintf(cmd, "nasm -f bin %s -o file.bin > /tmp/nasm-stdout 2> /tmp/nasm-stderr", flags_asm_filename(flags));

	int status = system(cmd);

	if (WEXITSTATUS(status) != 0) {
		fprintf(stderr, "error with nasm, see /tmp/nasm-stdout, /tmp/nasm-stderr \n");
		exit(EXIT_FAILURE);
	}

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

	struct sd_uc_engine* sduc = sd_uc_engine_from_mapped(mapped, filesize);

	ctx_dtor(ctx);

	return sduc;
}

struct sd_uc_engine* sd_uc_engine_from_tacbuffer(struct TACBuffer* buffer){
	return sd_uc_engine_from_tacbuffer_common(buffer, false, false);
}

struct sd_uc_engine* sd_uc_engine_from_tacbuffer_v2(struct TACBuffer* buffer, bool debug){
	return sd_uc_engine_from_tacbuffer_common(buffer, debug, false);
}

struct sd_uc_engine* sd_uc_engine_from_tacbuffer_v3(struct TACBuffer* buffer, bool debug, bool fake_lvst){
	return sd_uc_engine_from_tacbuffer_common(buffer, debug, fake_lvst);
}

uc_err sd_uc_mem_write64(struct sd_uc_engine* sduc, uint64_t address, const void* bytes) {
	return uc_mem_write(sduc->uc, address, bytes, sizeof(uint64_t));
}

uc_err sd_uc_mem_read64(struct sd_uc_engine* sduc, uint64_t address, void* bytes) {
	return uc_mem_read(sduc->uc, address, bytes, sizeof(uint64_t));
}

uc_err sd_uc_reg_read(struct sd_uc_engine* sduc, int regid, void* value) {
	return uc_reg_read(sduc->uc, regid, value);
}

bool sd_uc_some_reg_has_value(struct sd_uc_engine* sduc, uint64_t value) {

	uc_err err;
	//check that the value was written to any register
	bool found = false;

	int regs[] = {
	    UC_X86_REG_RAX,
	    UC_X86_REG_RBX,
	    UC_X86_REG_RCX,
	    UC_X86_REG_RDX,
	};

	uint64_t reg = 0;
	for (int i = 0; i < sizeof(regs) / sizeof(regs[0]); i++) {

		err = sd_uc_reg_read(sduc, regs[i], &reg);
		assert(err == UC_ERR_OK);
		if (reg == value) found = true;
	}

	return found;
}
