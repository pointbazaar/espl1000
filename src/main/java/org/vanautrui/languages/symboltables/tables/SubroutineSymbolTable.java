package org.vanautrui.languages.symboltables.tables;

import io.bretty.console.table.Alignment;
import io.bretty.console.table.ColumnFormatter;
import io.bretty.console.table.Precision;
import io.bretty.console.table.Table;
import org.vanautrui.languages.symboltables.rows.ISymbolTableRow;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class SubroutineSymbolTable implements ISymbolTable {

    private BaseSymbolTable symbolTable;

    public SubroutineSymbolTable(){
        this.symbolTable=new BaseSymbolTable();
    }

    public void add(ISymbolTableRow row) {
        this.symbolTable.add(row);
    }

    @Override
    public boolean containsVariable(String varName) {
        return this.symbolTable.containsVariable(varName);
    }

    @Override
    public int getIndexOfVariable(String varName)throws Exception {
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
    public List<ISymbolTableRow> getRows() {
        return symbolTable.getRows();
    }

    @Override
    public String toString(){

        // define a formatter for each column
        String[] names = this.symbolTable.getRows().stream().map(row->row.getName()).collect(Collectors.toList()).toArray(new String[]{});
        String[] types = this.symbolTable.getRows().stream().map(row->row.getType()).collect(Collectors.toList()).toArray(new String[]{});

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
