package org.vanautrui.languages.symboltables.rows;

public class LocalVarSymbolTableRow implements ISymbolTableRow {

    public final String varName;
    public final String typeName;


    public LocalVarSymbolTableRow(String varName, String typeName){

        this.typeName=typeName;
        this.varName=varName;
    }

    @Override
    public String toString(){

        return String.format("| %8s | %8s |",varName,typeName);
    }



    @Override
    public String getName() {
        return this.varName;
    }

    @Override
    public String getType() {
        return this.typeName;
    }
}
