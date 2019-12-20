package org.vanautrui.languages.compiler.symboltables;

import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import static org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTableRow.KIND_ARGUMENT;

public final class LocalVarSymbolTable  {

    private List<LocalVarSymbolTableRow> symbolTable;

    public LocalVarSymbolTable(){
        this.symbolTable=new ArrayList<>();
    }

    public void add_idempotent(final LocalVarSymbolTableRow row) {
        if(!this.containsVariable(row.getName())) {
            this.symbolTable.add(row);
        }
    }

    public LocalVarSymbolTableRow get(final String varName)throws Exception{
        for(int i=0;i<symbolTable.size();i++){
            LocalVarSymbolTableRow r = symbolTable.get(i);
            if(r.getName().equals(varName)){
                return r;
            }
        }
        throw new Exception("did not find symbol '"+varName+"' in symbol table");
    }

    public boolean containsVariable(final String varName) {
        return this.symbolTable
                .stream().anyMatch(r -> r.getName().equals(varName));
    }

    public int getIndexOfVariable(final String varName) throws Exception{
        //return get(varName).getIndex();
        final String mykind = getSegment(varName);
        final List<LocalVarSymbolTableRow> mykinds = this.symbolTable.stream().filter(row -> row.kind.equals(mykind)).collect(Collectors.toList());

        for(int i=0;i<mykinds.size();i++){
            final LocalVarSymbolTableRow row = mykinds.get(i);
            if(row.getName().equals(varName)){
                return i;
            }
        }
        throw new Exception("could not find the index of local variable "+varName);
    }

    public TypeNode getTypeOfVariable(final String varName) throws Exception{
        return this.get(varName).getType();
    }

    public String getSegment(final String name) throws Exception{
        //get the appropriate vm segment
        return get(name).kind;
    }

    public int size() {
        return this.symbolTable.size();
    }

    public long countLocals(){
        return this.symbolTable.stream().filter(r-> r.kind.equals(LocalVarSymbolTableRow.KIND_LOCALVAR)).count();
    }

    public long countArgs(){
        return this.symbolTable.stream().filter(r-> r.kind.equals(KIND_ARGUMENT)).count();
    }

    public List<LocalVarSymbolTableRow> getRows() {
        return new ArrayList<>(this.symbolTable);
    }

    @Override
    public String toString(){

        final String[] names = this.symbolTable.stream().map(LocalVarSymbolTableRow::getName).collect(Collectors.toList()).toArray(new String[]{});
        final String[] types = this.symbolTable.stream().map(row->row.getType().getTypeName()).collect(Collectors.toList()).toArray(new String[]{});
        final String[] kinds = this.symbolTable.stream().map(row->row.kind).collect(Collectors.toList()).toArray(new String[]{});

        final int[] indices_inner = IntStream.range(0,this.symbolTable.size()).toArray();
        final Integer[] indices = Arrays.stream( indices_inner ).boxed().toArray( Integer[]::new );

        final StringBuilder table = new StringBuilder();

        //append table headers
        table.append(String.format("%12s | %12s | %12s | %12s\n","Variable Name","Type","Index","Kind"));

        for(final int i : indices){

            final String varname = names[i];
            final String type = types[i];
            final int index = indices[i]; //just for pattern sake
            final String kind = kinds[i];

            //append a row
            table.append(String.format("%12s | %12s | %12d | %12s\n",varname,type,index,kind));
        }


        return "\n(METHOD SCOPE) VARIABLES SYMBOL TABLE: \n"+table.toString();
    }


    public TypeNode getReturnTypeOfSubroutineVariable(String subrName) throws Exception{
        //this variable is a subroutine. we want to know its return type from the local variable symbol table
        return this.get(subrName).getReturnTypeIfIsSubroutine();
    }
}
