package org.vanautrui.languages.compiler.symboltables.util;

import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;

public final class SymbolTableContext {
    /*
    This class has some of the other symbol tables as members.
    So that they don't have to be passed individually to the different code generation routines.
    It's members should be immutable
     */

    public final SubroutineSymbolTable subTable;
    public final LocalVarSymbolTable varTable;
    public final StructsSymbolTable structsTable;

    public SymbolTableContext(SubroutineSymbolTable subTable, LocalVarSymbolTable varTable, StructsSymbolTable structsTable) {
        this.subTable = subTable;
        this.varTable = varTable;
        this.structsTable = structsTable;
    }
}
