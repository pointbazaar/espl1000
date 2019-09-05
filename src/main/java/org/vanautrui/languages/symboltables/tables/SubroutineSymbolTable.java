package org.vanautrui.languages.symboltables.tables;

import io.bretty.console.table.Alignment;
import io.bretty.console.table.ColumnFormatter;
import io.bretty.console.table.Precision;
import io.bretty.console.table.Table;
import org.vanautrui.languages.symboltables.rows.SubroutineSymbolTableRow;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class SubroutineSymbolTable {

    private List<SubroutineSymbolTableRow> symbolTable;
    private int index_count=0;

    public SubroutineSymbolTable(){
        this.symbolTable=new ArrayList<>();
    }

    public void add(SubroutineSymbolTableRow row) {
        if(!this.containsVariable(row.getName())) {
            this.symbolTable.add(row);
            index_count++;
        }
    }

    public boolean containsVariable(String varName) {
        return symbolTable.stream().filter(e->e.getName().equals(varName)).collect(Collectors.toList()).size()>0;
    }

    public int getIndexOfVariable(String varName)throws Exception {
        if(!this.containsVariable(varName)){
            throw new Exception("could not get index of variable "+varName+" in symbol table. ");
        }

        for(int i=0;i<this.symbolTable.size();i++){
            SubroutineSymbolTableRow row = this.symbolTable.get(i);
            if(row.getName().equals(varName)){
                return i;
            }
        }
        throw new Exception();
    }

    public String getTypeOfVariable(String varName) {
        return symbolTable.stream().filter(e->e.getName().equals(varName)).collect(Collectors.toList()).get(0).getType();
    }

    public int size() {
        return this.symbolTable.size();
    }

    public String toString(){

        // define a formatter for each column
        String[] names = this.symbolTable.stream().map(row->row.getName()).collect(Collectors.toList()).toArray(new String[]{});
        String[] types = this.symbolTable.stream().map(row->row.getType()).collect(Collectors.toList()).toArray(new String[]{});

        int[] indices_inner = IntStream.range(0,this.symbolTable.size()).toArray();
        Integer[] indices = Arrays.stream( indices_inner ).boxed().toArray( Integer[]::new );

        ColumnFormatter<String> stringColumnFormatter = ColumnFormatter.text(Alignment.LEFT, 20);
        ColumnFormatter<Number> integerColumnFormatter = ColumnFormatter.number(Alignment.RIGHT, 7, Precision.ZERO);


        Table.Builder builder = new Table.Builder("Subroutine Name",names, stringColumnFormatter);


        builder.addColumn("Type", types, stringColumnFormatter);
        builder.addColumn("Index", indices, integerColumnFormatter);


        Table table = builder.build();
        //System.out.println(table); // NOTICE: table.toString() is called implicitly
        return "\nSUBROUTINE SYMBOL TABLE: \n"+table.toString();
    }
}
