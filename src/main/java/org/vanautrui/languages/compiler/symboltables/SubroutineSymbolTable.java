package org.vanautrui.languages.compiler.symboltables;

import io.bretty.console.table.Alignment;
import io.bretty.console.table.ColumnFormatter;
import io.bretty.console.table.Precision;
import io.bretty.console.table.Table;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;

import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public final class SubroutineSymbolTable {

    private List<SubroutineSymbolTableRow> symbolTable;

    public SubroutineSymbolTable()throws Exception{
        this.symbolTable=new ArrayList<>();

        //add the builtin subroutines

        //already implemented subroutines

        //putchar
        final List<TypeNode> putchar_arg_types = Arrays.asList(new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("Char"))));
        final SubroutineSymbolTableRow putchar = new SubroutineSymbolTableRow("putchar",new SimpleTypeNode("PInt"),"Builtin",0,putchar_arg_types);
        this.add(putchar);

        //putdigit
        final List<TypeNode> putdigit_arg_types = Arrays.asList(new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("PInt"))));
        final SubroutineSymbolTableRow putdigit = new SubroutineSymbolTableRow("putdigit",new SimpleTypeNode("PInt"),"Builtin",0,putdigit_arg_types);
        this.add(putdigit);

        //readchar
        final List<TypeNode> readchar_arg_types = new ArrayList<>();
        final SubroutineSymbolTableRow readchar = new SubroutineSymbolTableRow("readchar",new SimpleTypeNode("Char"),"Builtin",0,readchar_arg_types);
        this.add(readchar);



        //to be maybe implemented later
        /*
        this.add(new SubroutineSymbolTableRow("readint",new SimpleTypeNode("PInt"),"Builtin",0,1));
        this.add(new SubroutineSymbolTableRow("int2char",new SimpleTypeNode("Char"),"Builtin",0,1));
        this.add(new SubroutineSymbolTableRow("float2int",new SimpleTypeNode("Integer"),"Builtin",0,1));
        this.add(new SubroutineSymbolTableRow("int2float",new SimpleTypeNode("Float"),"Builtin",0,1));
        */

        //for structs, to be able to allocate them, and for arrays.
        //as per dracovm spec

        //new
        final List<TypeNode> new_arg_types = Arrays.asList(new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("PInt"))));
        this.add(new SubroutineSymbolTableRow("new",new SimpleTypeNode("#"),"Builtin",0,new_arg_types));

        //len
        final List<TypeNode> len_arg_types = Arrays.asList(new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("#"))));
        // arg type should really be [#], but not sure if i want to implement that yet.
        // dragon would then have to match nested types to find out if the types are compatible
        // i want to implement that later
        this.add(new SubroutineSymbolTableRow("len",new SimpleTypeNode("PInt"),"Builtin",0,len_arg_types));

        //abs
        final List<TypeNode> abs_arg_types = Arrays.asList(new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("Integer"))));
        this.add(new SubroutineSymbolTableRow("abs",new SimpleTypeNode("PInt"),"Builtin",0,abs_arg_types));

        //time
        final List<TypeNode> time_arg_types = new ArrayList<>();
        this.add(new SubroutineSymbolTableRow("time",new SimpleTypeNode("PInt"),"Builtin",0,time_arg_types));
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

    public TypeNode getReturnTypeOfSubroutine(String subroutineName) {
        return symbolTable.stream().filter(e->e.getName().equals(subroutineName)).collect(Collectors.toList()).get(0).getReturnType();
    }

    public int size() {
        return this.symbolTable.size();
    }

    public String toString(){

        // define a formatter for each column
        String[] names = this.symbolTable.stream().map(SubroutineSymbolTableRow::getName).collect(Collectors.toList()).toArray(new String[]{});
        String[] types = this.symbolTable.stream().map(row->row.getReturnType().getTypeName()).collect(Collectors.toList()).toArray(new String[]{});

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

    public TypeNode getTypeOfSubroutine(String subroutine_name) throws Exception{
        return this.get(subroutine_name).getReturnType();
    }

    public TypeNode getArgTypeOfSubroutineAtIndex(final String methodName, final int index) throws Exception {
        return get(methodName).getArgumentTypes().get(index);
    }
}
