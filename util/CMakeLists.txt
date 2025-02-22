add_library("sd-util" STATIC
	status/status.c
)

target_compile_options("sd-util" PUBLIC ${COMMON_OPTS})

