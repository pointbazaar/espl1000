
add_library("sd-tables" STATIC

	cc/cc.c

	lvst/lvst.c

	sst/sst.c
	sst/sst_fill.c
	sst/sst_print.c

        stst/stst.c
	stst/stst_print.c

	data/data.c

	enum/enum_table.c
	enum/enum_table.h

        symtable/symtable.c
)

target_include_directories("sd-tables" PUBLIC "..")

add_subdirectory(test)
