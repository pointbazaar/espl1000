package org.vanautrui.languages.compiler.symboltables;

public class LocalVarSymbolTableRow  {

    public final String varName;
    public final String typeName;

    public final String kind;
    public final int index;

    public final static String KIND_LOCALVAR="LOCAL";
    public final static String KIND_ARGUMENT="ARG";

    public LocalVarSymbolTableRow(String varName, String typeName,String kind,int index){

        this.typeName=typeName;
        this.varName=varName;
        this.kind=kind;
        this.index=index;
    }

    @Override
    public String toString(){

        return String.format("| %8s | %8s | %8s |",varName,typeName,kind);
    }

    public String getName() {
        return this.varName;
    }

    public String getType() {
        return this.typeName;
    }

    /**there may be 2 local variables and 2 local argument variables in a subroutine.
     * so there are
     * LOCAL 0,LOCAL 1,
     * and,
     * ARG 0, ARG 1
     * @return returns the index of the local variable in it's segment.
     */
    public int getIndex() {
        return this.index;
    }
}
