package org.vanautrui.languages.compiler.symboltables;

import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SubroutineTypeNode;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public final class SubroutineSymbolTable {

    private List<SubroutineSymbolTableRow> symbolTable;

    public SubroutineSymbolTable() {
        this.symbolTable = Collections.synchronizedList(new ArrayList<>());

        //add the builtin subroutines

        //already implemented subroutines

        //putchar
        final TypeNode putchar_returnType = new TypeNode(new SimpleTypeNode("PInt"));
        final List<TypeNode> putchar_arg_types = Arrays.asList(new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("Char"))));
        final SubroutineTypeNode putchar_type = new SubroutineTypeNode(putchar_arg_types, putchar_returnType,true);
        final SubroutineSymbolTableRow putchar = new SubroutineSymbolTableRow(
                "putchar",
                "Builtin",
                0,
                putchar_type
        );
        this.add(putchar);



        //putdigit
        final TypeNode putdigit_returnType = new TypeNode(new SimpleTypeNode("PInt"));
        final List<TypeNode> putdigit_arg_types = Arrays.asList(new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("PInt"))));
        final SubroutineTypeNode putdigit_type = new SubroutineTypeNode(putdigit_arg_types,putdigit_returnType,true);
        final SubroutineSymbolTableRow putdigit = new SubroutineSymbolTableRow("putdigit", "Builtin",0, putdigit_type);
        this.add(putdigit);

        //readchar
        final TypeNode readchar_returnType = new TypeNode(new SimpleTypeNode("Char"));
        final List<TypeNode> readchar_arg_types = new ArrayList<>();
        final SubroutineTypeNode readchar_type = new SubroutineTypeNode(readchar_arg_types,readchar_returnType,true);
        final SubroutineSymbolTableRow readchar = new SubroutineSymbolTableRow("readchar","Builtin",0,readchar_type);
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
        final TypeNode new_returnType = new TypeNode(new SimpleTypeNode("#"));
        final SubroutineTypeNode new_type = new SubroutineTypeNode(new_arg_types,new_returnType,false);
        this.add(new SubroutineSymbolTableRow("new","Builtin",0,new_type));

        //len
        final List<TypeNode> len_arg_types = Arrays.asList(new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("#"))));
        final TypeNode len_returnType = new TypeNode(new SimpleTypeNode("PInt"));
        final SubroutineTypeNode len_type = new SubroutineTypeNode(len_arg_types,len_returnType,false);
        // arg type should really be [#], but not sure if i want to implement that yet.
        // dragon would then have to match nested types to find out if the types are compatible
        // i want to implement that later
        this.add(new SubroutineSymbolTableRow("len","Builtin",0, len_type));

        //abs
        final List<TypeNode> abs_arg_types = Arrays.asList(new TypeNode(new BasicTypeWrappedNode(new SimpleTypeNode("Int"))));
        final TypeNode abs_returnType = new TypeNode(new SimpleTypeNode("PInt"));
        final SubroutineTypeNode abs_type = new SubroutineTypeNode(abs_arg_types,abs_returnType,false);
        this.add(new SubroutineSymbolTableRow("abs","Builtin",0, abs_type));

        //time
        final List<TypeNode> time_arg_types = new ArrayList<>();
        final TypeNode time_returnType = new TypeNode(new SimpleTypeNode("PInt"));
        final SubroutineTypeNode time_type = new SubroutineTypeNode(time_arg_types,time_returnType,true);
        this.add(new SubroutineSymbolTableRow("time","Builtin",0, time_type));
    }

    public synchronized void add(final SubroutineSymbolTableRow row) {
        if(!this.containsSubroutine(row.getName())) {
            this.symbolTable.add(row);
        }
    }

    public synchronized boolean containsSubroutine(final String subroutineName) {
        return symbolTable.stream().filter(e->e.getName().equals(subroutineName)).collect(Collectors.toList()).size()>0;
    }

    public synchronized int getIndexOfSubroutine(final String subroutineName)throws Exception {
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

    public synchronized TypeNode getReturnTypeOfSubroutine(final String subroutineName) throws Exception {
        return this.get(subroutineName).getReturnType();
    }

    public synchronized int size() {
        return this.symbolTable.size();
    }

    public synchronized String toString(){

        // define a formatter for each column
        final String[] names = this.symbolTable.stream().map(SubroutineSymbolTableRow::getName).collect(Collectors.toList()).toArray(new String[]{});

        final String[] returntypes = this.symbolTable.stream().map(row->row.getReturnType().getTypeName()).collect(Collectors.toList()).toArray(new String[]{});

        final String[] types = this.symbolTable.stream().map(row -> row.getType().getTypeName()).collect(Collectors.toList()).toArray(new String[]{});

        final int[] indices_inner = IntStream.range(0,this.symbolTable.size()).toArray();
        final Integer[] indices = Arrays.stream( indices_inner ).boxed().toArray( Integer[]::new );

        final StringBuilder table = new StringBuilder();

        final String formatString = "%18s | %12s | %17s | %12s \n";

        //append table headers
        table.append(String.format(formatString,"Subroutine Name","Return Type","Type","Index"));

        for(final int i : indices){

            final String varname = names[i];
            final String returntype = returntypes[i];
            final String type = types[i];
            final int index = indices[i]; //just for pattern sake

            //append a row
            table.append(String.format(formatString,varname,returntype,type,index));
        }
        //System.out.println(table); // NOTICE: table.toString() is called implicitly
        return "\nSUBROUTINE SYMBOL TABLE: \n"+table.toString();
    }

    public synchronized String getContainingClassName(final String identifierMethodName) throws Exception{
        return this.get(identifierMethodName).getClassName();
    }

    private synchronized SubroutineSymbolTableRow get(final String methodName) throws Exception{
        for (final SubroutineSymbolTableRow row : this.symbolTable) {
            if (row.getName().equals(methodName)) {
                return row;
            }
        }
        throw new Exception("could not find "+methodName+" in subroutine symbol table");
    }

    public synchronized int getNumberOfLocalVariablesOfSubroutine(final String methodName) throws Exception{
        return this.get(methodName).getNumberOfLocalVariables();
    }

    public synchronized int getNumberOfArgumentsOfSubroutine(final String methodName) throws Exception{
        return this.get(methodName).getNumberOfArguments();
    }

    public synchronized TypeNode getTypeOfSubroutine(final String subroutineName) throws Exception{

        return this.get(subroutineName).getType();
    }

    public synchronized TypeNode getArgTypeOfSubroutineAtIndex(final String methodName, final int index) throws Exception {
        return get(methodName).getArgumentTypes().get(index);
    }
}
