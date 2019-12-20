package org.vanautrui.languages.compiler.symboltables.structs;

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

    public final void addIfNotAlreadyKnown(final StructsSymbolTableRow row){
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

        final int[] indices_inner = IntStream.range(0,this.size()).toArray();
        final Integer[] indices = Arrays.stream( indices_inner ).boxed().toArray( Integer[]::new );


        final StringBuilder table = new StringBuilder();

        //append table headers
        table.append(String.format("%12s | %12s | %12s | %12s\n","Struct Name","Field Count","Index"));

        for(final int i : indices){

            final String structname = names[i];
            final int fieldcount = count_elements[i];
            final int index = indices[i];

            //append a row
            table.append(String.format("%12s | %12s | %12d | %12s\n",structname,fieldcount,index));
        }

        return "\nSTRUCT SYMBOL TABLE: \n"+table.toString();
    }
}
