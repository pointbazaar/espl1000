
set(ESPL_STDLIB_PATH "${CMAKE_SOURCE_DIR}/stdlib")
add_definitions(-DESPL_STDLIB_PATH="${ESPL_STDLIB_PATH}")

add_library("sd-base"
	compiler.c
	compile_single_function.c

	#analyzer
	analyzer/annotation/annotation_analyzer.c
	analyzer/dead/dead_analyzer.c
	analyzer/fn/fn_analyzer.c
	analyzer/halts/halt_analyzer.c
	analyzer/lv/lv_analyzer.c
	analyzer/data/data_analyzer.c

	#liveness
	liveness/liveness.c

	#code_gen/avr_code_gen
	avr_code_gen/cg_avr.c

	gen_tac/gen_tac.c
	gen_tac/gen_tac_assignstmt.c
	gen_tac/gen_tac_call.c
	gen_tac/gen_tac_constvalue.c
	gen_tac/gen_tac_const_data.c
	gen_tac/gen_tac_expr.c
	gen_tac/gen_tac_forstmt.c
	gen_tac/gen_tac_ifstmt.c
	gen_tac/gen_tac_retstmt.c
	gen_tac/gen_tac_simplevar.c
	gen_tac/gen_tac_stmt.c
	gen_tac/gen_tac_term.c
	gen_tac/gen_tac_unopterm.c
	gen_tac/gen_tac_deref.c
	gen_tac/gen_tac_address_of.c
	gen_tac/gen_tac_variable.c
	gen_tac/gen_tac_whilestmt.c
	gen_tac/helper_gen_tac_derefll.c

		avr_code_gen/compile_ir/compile_tac.h
		avr_code_gen/compile_ir/compile_tac_binary_op.c
		avr_code_gen/compile_ir/compile_tac_call.c
		avr_code_gen/compile_ir/compile_tac_const_value.c
		avr_code_gen/compile_ir/compile_tac_copy.c
		avr_code_gen/compile_ir/compile_tac_goto.c
		avr_code_gen/compile_ir/compile_tac_if_goto.c
		avr_code_gen/compile_ir/compile_tac_label.c
		avr_code_gen/compile_ir/compile_tac_nop.c
		avr_code_gen/compile_ir/compile_tac_return.c
		avr_code_gen/compile_ir/compile_tac_unary_op.c
		avr_code_gen/compile_ir/compile_tac_param.c
		avr_code_gen/compile_ir/compile_tac_setup_stackframe.c
		avr_code_gen/compile_ir/compile_tac_load_local_addr.c
		avr_code_gen/compile_ir/compile_tac_store_local.c
		avr_code_gen/compile_ir/compile_tac_if_cmp_goto.c
		avr_code_gen/compile_ir/compile_tac_load.c
		avr_code_gen/compile_ir/compile_tac_store.c
		avr_code_gen/compile_ir/compile_tac_setup_sp.c

	# x86 backend
	x86_code_gen/cg_x86.c
	x86_code_gen/cg_x86_basic_block.c
	x86_code_gen/cg_x86_single_tac.c
	x86_code_gen/allocate_registers_x86.c
	x86_code_gen/compile_ir/compile_tac.h
	x86_code_gen/compile_ir/compile_tac_binary_op.c
	x86_code_gen/compile_ir/compile_tac_call.c
	x86_code_gen/compile_ir/compile_tac_icall.c
	x86_code_gen/compile_ir/compile_tac_const_value.c
	x86_code_gen/compile_ir/compile_tac_const_data.c
	x86_code_gen/compile_ir/compile_tac_copy.c
	x86_code_gen/compile_ir/compile_tac_goto.c
	x86_code_gen/compile_ir/compile_tac_if_goto.c
	x86_code_gen/compile_ir/compile_tac_label.c
	x86_code_gen/compile_ir/compile_tac_nop.c
	x86_code_gen/compile_ir/compile_tac_return.c
	x86_code_gen/compile_ir/compile_tac_unary_op.c
	x86_code_gen/compile_ir/compile_tac_param.c
	x86_code_gen/compile_ir/compile_tac_setup_stackframe.c
	x86_code_gen/compile_ir/compile_tac_load_local_addr.c
	x86_code_gen/compile_ir/compile_tac_store_local.c
	x86_code_gen/compile_ir/compile_tac_if_cmp_goto.c
	x86_code_gen/compile_ir/compile_tac_load.c
	x86_code_gen/compile_ir/compile_tac_store.c
	x86_code_gen/compile_ir/compile_tac_load_function_ptr.c

	# Linux syscall definitions
	x86_code_gen/syscalls/syscalls.c
	x86_code_gen/syscalls/table.c

	#DerefLL implementation
	derefll/derefll.c

	basic_block/basicblock.c
	basic_block/basicblock_printer.c

	avr_code_gen/cg_avr_basic_block.c
	avr_code_gen/cg_avr_single_tac.c

	#typechecker
	typechecker/typecheck.c
	typechecker/util/tc_errors.c
	typechecker/tc_stmts.c
	typechecker/tc_methodcall.c
	typechecker/tc_assignstmt.c
	typechecker/tc_local_var_decl_stmt.c
	typechecker/tc_expr.c
	typechecker/tc_unopterm.c
	typechecker/tc_address_of.c
	typechecker/tc_deref.c
	typechecker/tc_term.c
	typechecker/tc_var.c
	typechecker/tc_simplevar.c
	typechecker/util/tc_utils.c
	typechecker/tc_range.c
	typechecker/tc_method.c

	# typechecker: type contains
	typechecker/type_contains/tc_type_contains.c
	typechecker/type_contains/tc_simpletype_contains.c
	typechecker/type_contains/tc_subrtype_contains.c
	typechecker/type_contains/tc_structtype_contains.c
	typechecker/type_contains/tc_primitivetype_contains.c
	typechecker/type_contains/tc_pointertype_contains.c
	typechecker/type_contains/tc_basictype_contains.c
	typechecker/type_contains/tc_arraytype_contains.c
	typechecker/type_contains/tc_type_contains.h

	typechecker/tc_retstmt.c
	typechecker/tc_ifstmt.c
	typechecker/tc_whilestmt.c
	typechecker/tc_forstmt.c

	#typeinference
	typeinference/typeinfer.c
	typeinference/typeinfer_expr.c
	typeinference/typeinfer_methodcall.c
	typeinference/typeinfer_simplevar.c
	typeinference/typeinfer_term.c
	typeinference/typeinfer_deref.c
	typeinference/typeinfer_address_of.c
	typeinference/typeinfer_lvalue.c
	typeinference/typeinfer_var.c
	typeinference/typeinfer_const.c
	typeinference/util/type_str.c
	typeinference/infer_in_context.c

	#util
	util/fill_tables.c
	util/fill_tables.h
	util/fileutils/fileutils.c
	util/ctx.c

	#flags
	cli/flags/flags.c
	cli/flags/all_flags.c
	cli/flags/validate_flags.c
)

add_executable("sd"
	cli/main.c
)

target_link_libraries("sd-base" lexer-lib)
#tables dependency
target_link_libraries("sd-base" sd-tables)
#RAT dependency
target_link_libraries("sd-base" sd-rat)
#TAC dependency
target_link_libraries("sd-base" sd-tac)
#Parser dependency
target_link_libraries("sd-base" dragon-parser-base)
#IBuffer Dependency
target_link_libraries("sd-base" sd-ibuffer)
#AST dependency
target_link_libraries("sd-base" sd-ast)
#Util dependency
target_link_libraries("sd-base" sd-util)
#Token dependency
target_link_libraries("sd-base" sd-token)

target_link_libraries("sd" sd-base)

target_include_directories("sd-base" PUBLIC ".")
target_include_directories("sd-base" PUBLIC "../..")

target_include_directories("sd" PUBLIC "../main")
target_include_directories("sd" PUBLIC "../..")

