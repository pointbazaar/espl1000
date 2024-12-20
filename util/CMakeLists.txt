add_library("sd-util" STATIC
	exit_malloc/exit_malloc.c
	status/status.c
)

target_compile_options("sd-util" PUBLIC ${COMMON_OPTS})

