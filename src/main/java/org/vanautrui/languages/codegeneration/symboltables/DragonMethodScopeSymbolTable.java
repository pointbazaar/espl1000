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

}
