package org.vanautrui.languages.codegeneration.symboltables.tables;

import org.vanautrui.languages.codegeneration.symboltables.rows.ISymbolTableRow;

public class DragonMethodScopeVariableSymbolTable implements ISymbolTable {

    private DragonBaseSymbolTable symbolTable;

    public DragonMethodScopeVariableSymbolTable(){
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
        return this.symbolTable.toString();
    }
}
