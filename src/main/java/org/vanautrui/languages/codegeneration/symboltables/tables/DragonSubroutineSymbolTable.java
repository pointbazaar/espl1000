package org.vanautrui.languages.codegeneration.symboltables.tables;

import org.vanautrui.languages.codegeneration.symboltables.rows.ISymbolTableRow;

public class DragonSubroutineSymbolTable implements ISymbolTable {

    private DragonBaseSymbolTable symbolTable;

    public DragonSubroutineSymbolTable(){
        this.symbolTable=new DragonBaseSymbolTable();
    }

    @Override
    public void add(ISymbolTableRow row) {
        this.symbolTable.add(row);
    }

    @Override
    public boolean containsVariable(String varName) {
        return this.symbolTable.containsVariable(varName);
    }

    @Override
    public int getIndexOfVariable(String varName) {
        return this.symbolTable.getIndexOfVariable(varName);
    }

    @Override
    public String getTypeOfVariable(String varName) {
        return this.symbolTable.getTypeOfVariable(varName);
    }

    @Override
    public int size() {
        return this.symbolTable.size();
    }

    @Override
    public String toString(){
        StringBuilder result = new StringBuilder("SUBROUTINE SYMBOL TABLE: \n");
        result.append("| name | type |\n");
        for(ISymbolTableRow row : this.symbolTable.getRows()){
            result.append(row.toString());
            result.append("\n");
        }
        return result.toString();
    }
}
