
add_executable("sd-tests"
	#tests
	test.c

	#typeinference
	typeinference/test_typeinference.c
	typeinference/test_typeinference_util.c
	typeinference/test_typeinference_util.h

	#typechecker
	typechecker/test_typechecker_util.c
	typechecker/test_typechecker_util.h
	typechecker/test_typechecker.c
	typechecker/test_typechecker.h

	#libvmcu utils
	libvmcu_utils/libvmcu_utils.c

	#CodeGen from TAC
	avr_code_gen/test_avr_code_gen.c
	avr_code_gen/test_avr_code_gen.h
	avr_code_gen/test_avr_code_gen_util.c
	avr_code_gen/test_avr_code_gen_util.h

	avr_code_gen/compile_ir/test_compile_tac_binary_op.c
	avr_code_gen/compile_ir/test_compile_tac_binary_op_immediate.c
	avr_code_gen/compile_ir/test_compile_tac_const_value.c
	avr_code_gen/compile_ir/test_compile_tac_copy.c
	avr_code_gen/compile_ir/test_compile_tac_goto.c
	avr_code_gen/compile_ir/test_compile_tac_if_goto.c
	avr_code_gen/compile_ir/test_compile_tac_load_const_addr.c
	avr_code_gen/compile_ir/test_compile_tac_nop.c
	avr_code_gen/compile_ir/test_compile_tac_param.c
	avr_code_gen/compile_ir/test_compile_tac_return.c
	avr_code_gen/compile_ir/test_compile_tac_store_const_addr.c
	avr_code_gen/compile_ir/test_compile_tac_unary_op.c
	avr_code_gen/compile_ir/test_compile_tac_call.c
	avr_code_gen/compile_ir/test_compile_tac_if_cmp_goto.c
	avr_code_gen/compile_ir/test_compile_tac_load.c
	avr_code_gen/compile_ir/test_compile_tac_store.c
	avr_code_gen/compile_ir/test_compile_tac_setup_sp.c
	avr_code_gen/compile_ir/test_compile_tac_setup_stackframe.c

	#CodeGen from TAC - Timers
	avr_code_gen/timer/test_avr_code_gen_timer.c

	#CodeGen from TAC - x86
	x86_code_gen/test_x86_code_gen.c
	x86_code_gen/test_x86_code_gen.h
	x86_code_gen/test_x86_code_gen_util.c
	x86_code_gen/test_x86_code_gen_util.h
	x86_code_gen/fake_lvst.c
	x86_code_gen/compile_ir/test_compile_tac_copy.c
	x86_code_gen/compile_ir/test_compile_tac_goto.c
	x86_code_gen/compile_ir/test_compile_tac_nop.c
	x86_code_gen/compile_ir/test_compile_tac_const_value.c
	x86_code_gen/compile_ir/test_compile_tac_store_const_addr.c
	x86_code_gen/compile_ir/test_compile_tac_store.c
	x86_code_gen/compile_ir/test_compile_tac_load_const_addr.c
	x86_code_gen/compile_ir/test_compile_tac_load.c
	x86_code_gen/compile_ir/test_compile_tac_unary_op.c
	x86_code_gen/compile_ir/test_compile_tac_binary_op.c
	x86_code_gen/compile_ir/test_compile_tac_binary_op_immediate.c
	x86_code_gen/compile_ir/test_compile_tac_if_cmp_goto.c
	x86_code_gen/compile_ir/test_compile_tac_if_goto.c
	x86_code_gen/compile_ir/test_compile_tac_load_local.c
	x86_code_gen/compile_ir/test_compile_tac_store_local.c
	x86_code_gen/compile_ir/test_compile_tac_return.c
	x86_code_gen/compile_ir/test_compile_tac_param.c
	x86_code_gen/compile_ir/test_compile_tac_call.c

	#CodeGen from TAC - Timers
	avr_code_gen/timer/test_avr_code_gen_timer.c

	#CodeGen from Snippets (test compiling AST pieces)
	gen_tac/test_gen_tac.c
	gen_tac/util/test_gen_tac_util.c
	gen_tac/test_gen_tac_assignstmt.c
	gen_tac/test_gen_tac_mdirect.c
	gen_tac/test_gen_tac_expr.c
	gen_tac/test_gen_tac_ifstmt.c
	gen_tac/test_gen_tac_whilestmt.c
	gen_tac/test_gen_tac_forstmt.c
	gen_tac/test_gen_tac_massignstmt.c
	gen_tac/test_gen_tac_simplevar.c
	gen_tac/test_gen_tac_variable.c
	gen_tac/test_gen_tac_call.c
	gen_tac/test_gen_tac_structdecl.c
)

target_compile_options("sd-tests" PUBLIC ${COMMON_OPTS})

target_link_libraries("sd-tests" ${CMAKE_SOURCE_DIR}/dependencies/libvmcu-Virtual-MCU-Library/build-release/libvmcu.a "-lm")

add_library(unicorn SHARED IMPORTED)
set_target_properties(unicorn PROPERTIES
	IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/dependencies/unicorn/build/libunicorn.so
)
target_link_libraries("sd-tests" unicorn)

target_link_libraries("sd-tests" sd-base)

target_include_directories("sd-tests" PUBLIC "../main")
target_include_directories("sd-tests" PUBLIC ".")
target_include_directories("sd-tests" PUBLIC "../..")

#libvmcu include dirs for sd-tests
target_include_directories("sd-tests" PUBLIC "../../dependencies/libvmcu-Virtual-MCU-Library/engine/include")

target_include_directories("sd-tests" PUBLIC "../../dependencies/unicorn/include")


