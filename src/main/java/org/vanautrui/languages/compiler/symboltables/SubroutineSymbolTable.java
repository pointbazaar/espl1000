package org.vanautrui.languages.compiler.symboltables;

import io.bretty.console.table.Alignment;
import io.bretty.console.table.ColumnFormatter;
import io.bretty.console.table.Precision;
import io.bretty.console.table.Table;

import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class SubroutineSymbolTable {

    private List<SubroutineSymbolTableRow> symbolTable;

    public SubroutineSymbolTable(){
        this.symbolTable=new ArrayList<>();

        //add the builtin subroutines

        //already implemented subroutines
        this.add(new SubroutineSymbolTableRow("putchar","PInt","Builtin",0,1));
        this.add(new SubroutineSymbolTableRow("putdigit","PInt","Builtin",0,1));
        this.add(new SubroutineSymbolTableRow("readchar","Char","Builtin",0,0));

        //to be implemented later
        this.add(new SubroutineSymbolTableRow("readint","PInt","Builtin",0,1));
        this.add(new SubroutineSymbolTableRow("int2char","Char","Builtin",0,1));
        this.add(new SubroutineSymbolTableRow("float2int","Integer","Builtin",0,1));
        this.add(new SubroutineSymbolTableRow("int2float","Float","Builtin",0,1));
    }

    public void add(SubroutineSymbolTableRow row) {
        if(!this.containsSubroutine(row.getName())) {
            this.symbolTable.add(row);
        }
    }

    public boolean containsSubroutine(String subroutineName) {
        return symbolTable.stream().filter(e->e.getName().equals(subroutineName)).collect(Collectors.toList()).size()>0;
    }

    public int getIndexOfSubroutine(String subroutineName)throws Exception {
        if(!this.containsSubroutine(subroutineName)){
            throw new Exception("could not get index of variable "+subroutineName+" in symbol table. ");
        }

        for(int i=0;i<this.symbolTable.size();i++){
            SubroutineSymbolTableRow row = this.symbolTable.get(i);
            if(row.getName().equals(subroutineName)){
                return i;
            }
        }
        throw new Exception();
    }

    public String getReturnTypeOfSubroutine(String subroutineName) {
        return symbolTable.stream().filter(e->e.getName().equals(subroutineName)).collect(Collectors.toList()).get(0).getType();
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

    public String getContainingClassName(String identifierMethodName) throws Exception{
        return this.get(identifierMethodName).getClassName();
    }

    private SubroutineSymbolTableRow get(String methodName) throws Exception{
        for(int i=0;i<this.symbolTable.size();i++){
            SubroutineSymbolTableRow row = this.symbolTable.get(i);
            if(row.getName().equals(methodName)){
                return row;
            }
        }
        throw new Exception("could not find "+methodName+" in subroutine symbol table");
    }

    public int getNumberOfLocalVariablesOfSubroutine(String methodName) throws Exception{
        return this.get(methodName).getNumberOfLocalVariables();
    }

    public int getNumberOfArgumentsOfSubroutine(String methodName) throws Exception{
        return this.get(methodName).getNumberOfArguments();
    }
}
