package org.vanautrui.languages.compiler.symboltables.structs;

import io.bretty.console.table.Alignment;
import io.bretty.console.table.ColumnFormatter;
import io.bretty.console.table.Precision;
import io.bretty.console.table.Table;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTableRow;

import java.util.Arrays;
import java.util.HashSet;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public final class StructsSymbolTable {

    //this class shuold be a collection of all the structs in a program, represented by symbol table rows.
    //this table can be built prior to typechecking and vm code generation.
    //it can be used for typechecking and symbol table generation,

    public StructsSymbolTable(){

    }

    private final HashSet<StructsSymbolTableRow> elements=new HashSet<>();

    public final int size(){
        return this.elements.size();
    }

    public final boolean containsStructDefinition(final String mystructName){
        return this.elements.stream().filter(sstr -> sstr.structName.equals(mystructName)).count()>0;
    }

    public final void addIfNotAlreadyKnown(StructsSymbolTableRow row){
        if(!this.containsStructDefinition(row.structName)){
            this.elements.add(row);
        }
    }

    public final StructsSymbolTableRow get(final String name) throws Exception{

        if (containsStructDefinition(name)) {
            return this.elements.stream().filter(sstr -> sstr.structName.equals(name)).findFirst().get();
        }
        throw new Exception("could not find struct definition: "+name);
    }

    public final String toString(){

        // define a formatter for each column
        final String[] names = this.elements.stream().map(row->row.structName).collect(Collectors.toList()).toArray(new String[]{});
        final Integer[] count_elements = this.elements.stream().map(row->row.members.size()).collect(Collectors.toList()).toArray(new Integer[]{});

        //final int[] indices_inner = IntStream.range(0,this.size()).toArray();
        //Integer[] indices = Arrays.stream( indices_inner ).boxed().toArray( Integer[]::new );

        final ColumnFormatter<String> stringColumnFormatter = ColumnFormatter.text(Alignment.LEFT, 20);
        final ColumnFormatter<Number> integerColumnFormatter = ColumnFormatter.number(Alignment.RIGHT, 7, Precision.ZERO);


        final Table.Builder builder = new Table.Builder("Subroutine Name",names, stringColumnFormatter);


        builder.addColumn("#Members", count_elements, integerColumnFormatter);
        //builder.addColumn("Index", indices, integerColumnFormatter);


        final Table table = builder.build();
        //System.out.println(table); // NOTICE: table.toString() is called implicitly
        return "\nSTRUCT SYMBOL TABLE: \n"+table.toString();
    }
}
