package org.vanautrui.languages.compiler.symboltables;

import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.SubroutineTypeNode;

public class LocalVarSymbolTableRow  {

    //TODO: in the future there should be 2 classes for a row, and they should both implement an interface.
    //TODO: so that we can easily distingush between subroutine variables and normal variables and dont have so many optionals

    private final String varName;
    private final ITypeNode typeName; //this always contains the full type name

    final String kind;
    public final int index;

    public final static String KIND_LOCALVAR="LOCAL";
    public final static String KIND_ARGUMENT="ARG";

    public LocalVarSymbolTableRow(String varName, ITypeNode typeName,String kind,int index) {
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

    public ITypeNode getType() {
        return this.typeName;
    }

    public ITypeNode getReturnTypeIfIsSubroutine()throws Exception{
        if(this.typeName instanceof SubroutineTypeNode){
            return ((SubroutineTypeNode) this.typeName).returnType;
        }else {
            throw new Exception(this.getClass().getSimpleName()
                    +": tried to get return type of subroutine variable "
                    +this.varName+" but it is not a subroutine variable. its type is "
                    +this.getType());
        }
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
