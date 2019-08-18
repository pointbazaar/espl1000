package org.vanautrui.languages.codegeneration.symboltables;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.stream.Collectors;

public class DragonMethodScopeSymbolTable {

    private List<DragonMethodScopeSymbolTableRow> symbolTable= new ArrayList<>();

    private int index_count=0;

    public DragonMethodScopeSymbolTable(){}

    public void add(String varName,String typeName){
        //DEBUG
        System.out.println("symbol table add");
        if(!this.containsVariable(varName)) {
            this.symbolTable.add(new DragonMethodScopeSymbolTableRow(index_count, varName, typeName));
            index_count++;
        }
    }

    public boolean containsVariable(String varName){
        return symbolTable.stream().filter(e->e.varName.equals(varName)).collect(Collectors.toList()).size()>0;
    }

    public int getIndexOfVariable(String varName){
        return symbolTable.stream().filter(e->e.varName.equals(varName)).collect(Collectors.toList()).get(0).index;
    }

    @Override
    public String toString(){
        StringBuilder result = new StringBuilder("SYMBOL TABLE: \n");
        for(DragonMethodScopeSymbolTableRow row : this.symbolTable){
            result.append(row.toString());
            result.append("\n");
        }
        return result.toString();
    }

    public String getTypeOfVariable(String varName) {
        return symbolTable.stream().filter(e->e.varName.equals(varName)).collect(Collectors.toList()).get(0).typeName;
    }

    public int size() {
        return this.symbolTable.size();
    }
}
