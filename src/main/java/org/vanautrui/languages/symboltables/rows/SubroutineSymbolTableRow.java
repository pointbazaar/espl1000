package org.vanautrui.languages.symboltables.rows;

public class SubroutineSymbolTableRow implements ISymbolTableRow {

    public final String subRoutineName;
    public final String typeName;

    public SubroutineSymbolTableRow(String subRoutineName, String typeName){

        this.typeName=typeName;
        this.subRoutineName = subRoutineName;
    }

    @Override
    public String toString(){

        return String.format("| %8s | %8s |", subRoutineName,typeName);
    }


    @Override
    public String getName() {
        return this.subRoutineName;
    }

    @Override
    public String getType() {
        return this.typeName;
    }
}
