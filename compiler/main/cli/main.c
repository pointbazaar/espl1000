#include <stdlib.h>
#include <malloc.h>

#include "flags/flags.h"
#include "util/fileutils/fileutils.h"

#include "rat/rat.h"

#include "compiler.h"

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

static int rat_main(struct Flags* flags) {

	enum RAT_ARCH arch = RAT_ARCH_AVR;

	if (flags_x86(flags)) {
		arch = RAT_ARCH_X86;
	}

	struct RAT* rat = rat_ctor(RAT_ARCH_X86, 0);

	rat_print(rat);

	return 0;
}

int main(int argc, char* argv[]) {

	mallopt(M_CHECK_ACTION, 3);

	struct Flags* flags = makeFlags(argc, argv);

	if (!check_filenames_lowercase(flags)) {
		return EXIT_FAILURE;
	}

	if (flags_help(flags)) {
		sd_print_help();
		freeFlags(flags);
		return EXIT_SUCCESS;
	}

	if (flags_version(flags)) {
		printf("v0.2.0\n");
		freeFlags(flags);
		return EXIT_SUCCESS;
	}

	if (flags_print_filenames(flags)) {
		printf("%s\n", flags_asm_filename(flags));
		printf("%s\n", flags_token_filename(flags));
		printf("%s\n", flags_hex_filename(flags));
		printf("%s\n", flags_obj_filename(flags));
	}

	if (flags_rat(flags)) {
		return rat_main(flags);
	}

	bool success = compile(flags);

	return (success) ? EXIT_SUCCESS : EXIT_FAILURE;
}
