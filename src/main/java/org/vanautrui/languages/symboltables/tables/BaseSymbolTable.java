package org.vanautrui.languages.symboltables.tables;

import org.vanautrui.languages.symboltables.rows.ISymbolTableRow;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

//this class intentionally only has
//package level visibility so it is only used
//for composing symbol tables in this package
class BaseSymbolTable {

    private List<ISymbolTableRow> symbolTable= new ArrayList<>();

    private int index_count=0;

    public BaseSymbolTable(){}

    public void add(ISymbolTableRow row){
        //DEBUG
        //System.out.println("symbol table add");

        if(!this.containsVariable(row.getName())) {
            this.symbolTable.add(row);
            index_count++;
        }
    }

    public boolean containsVariable(String varName){
        return symbolTable.stream().filter(e->e.getName().equals(varName)).collect(Collectors.toList()).size()>0;
    }

    public int getIndexOfVariable(String varName)throws Exception{

        if(!this.containsVariable(varName)){
            throw new Exception("could not get index of variable "+varName+" in symbol table. ");
        }

        for(int i=0;i<this.symbolTable.size();i++){
            ISymbolTableRow row = this.symbolTable.get(i);
            if(row.getName().equals(varName)){
                return i;
            }
        }
        return -1;
    }

    @Override
    public String toString(){
        StringBuilder result = new StringBuilder("SYMBOL TABLE: \n");
        for(ISymbolTableRow row : this.symbolTable){
            result.append(row.toString());
            result.append("\n");
        }
        return result.toString();
    }

    public String getTypeOfVariable(String varName) {
        return symbolTable.stream().filter(e->e.getName().equals(varName)).collect(Collectors.toList()).get(0).getType();
    }

    public int size() {
        return this.symbolTable.size();
    }

    public List<ISymbolTableRow> getRows(){
        return this.symbolTable;
    }
}
