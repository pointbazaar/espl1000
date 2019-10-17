package org.vanautrui.languages.compiler.symboltables;

import io.bretty.console.table.Alignment;
import io.bretty.console.table.ColumnFormatter;
import io.bretty.console.table.Precision;
import io.bretty.console.table.Table;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import static org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTableRow.KIND_ARGUMENT;

public class LocalVarSymbolTable  {

    private List<LocalVarSymbolTableRow> symbolTable;

    public LocalVarSymbolTable(){
        this.symbolTable=new ArrayList<>();
    }

    public void add_idempotent(LocalVarSymbolTableRow row) {
        if(!this.containsVariable(row.getName())) {
            this.symbolTable.add(row);
        }
    }

    public LocalVarSymbolTableRow get(String varName)throws Exception{
        for(int i=0;i<symbolTable.size();i++){
            LocalVarSymbolTableRow r = symbolTable.get(i);
            if(r.getName().equals(varName)){
                return r;
            }
        }
        throw new Exception("did not find symbol '"+varName+"' in symbol table");
    }

    public boolean containsVariable(String varName) {
        return this.symbolTable.stream().filter(r->r.getName().equals(varName)).count()>0;
    }

    public int getIndexOfVariable(String varName) throws Exception{
        //return get(varName).getIndex();
        final String mykind = getSegment(varName);
        final List<LocalVarSymbolTableRow> mykinds = this.symbolTable.stream().filter(row -> row.kind.equals(mykind)).collect(Collectors.toList());

        for(int i=0;i<mykinds.size();i++){
            LocalVarSymbolTableRow row = mykinds.get(i);
            if(row.getName().equals(varName)){
                return i;
            }
        }
        throw new Exception("could not find the index of local variable "+varName);
    }

    public TypeNode getTypeOfVariable(String varName) throws Exception{
        return this.get(varName).getType();
    }

    public String getSegment(String name) throws Exception{
        //get the appropriate vm segment
        return get(name).kind;
    }

    public int size() {
        return this.symbolTable.size();
    }

    public long countLocals(){
        return this.symbolTable.stream().filter(r->r.kind==LocalVarSymbolTableRow.KIND_LOCALVAR).count();
    }

    public long countArgs(){
        return this.symbolTable.stream().filter(r->r.kind== KIND_ARGUMENT).count();
    }

    public List<LocalVarSymbolTableRow> getRows() {
        return this.symbolTable.stream().collect(Collectors.toList());
    }

    @Override
    public String toString(){

        String[] names = this.symbolTable.stream().map(row->row.getName()).collect(Collectors.toList()).toArray(new String[]{});
        String[] types = this.symbolTable.stream().map(row->row.getType().getTypeName()).collect(Collectors.toList()).toArray(new String[]{});
        String[] kinds = this.symbolTable.stream().map(row->row.kind).collect(Collectors.toList()).toArray(new String[]{});

        int[] indices_inner = IntStream.range(0,this.symbolTable.size()).toArray();
        Integer[] indices = Arrays.stream( indices_inner ).boxed().toArray( Integer[]::new );

        ColumnFormatter<String> stringColumnFormatter = ColumnFormatter.text(Alignment.LEFT, 20);
        ColumnFormatter<Number> integerColumnFormatter = ColumnFormatter.number(Alignment.RIGHT, 7, Precision.ZERO);


        Table.Builder builder = new Table.Builder("Variable Name",names, stringColumnFormatter);


        builder.addColumn("Type", types, stringColumnFormatter);
        builder.addColumn("Index", indices, integerColumnFormatter);
        builder.addColumn("Kind", kinds, stringColumnFormatter);


        Table table = builder.build();
        //System.out.println(table); // NOTICE: table.toString() is called implicitly
        return "\n(METHOD SCOPE) VARIABLES SYMBOL TABLE: \n"+table.toString();
    }


    public TypeNode getReturnTypeOfSubroutineVariable(String subrName) throws Exception{
        //this variable is a subroutine. we want to know its return type from the local variable symbol table
        return this.get(subrName).getReturnTypeIfIsSubroutine();
    }
}
