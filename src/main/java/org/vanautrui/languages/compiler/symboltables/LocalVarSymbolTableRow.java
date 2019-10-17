package org.vanautrui.languages.compiler.symboltables;

import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SubroutineTypeNode;

public class LocalVarSymbolTableRow  {

    //in the future there should be 2 classes for a row, and they should both implement an interface.
    //so that we can easily distingush between subroutine variables and normal variables and dont have so many optionals

    private final String varName;
    private final TypeNode typeName; //this always contains the full type name

    final String kind;

    public final static String KIND_LOCALVAR="LOCAL";
    public final static String KIND_ARGUMENT="ARG";
    public LocalVarSymbolTableRow(String varName, TypeNode typeName, String kind) {
        this.typeName=typeName;
        this.varName=varName;
        this.kind=kind;
    }
    public LocalVarSymbolTableRow(String varName, IBasicAndWrappedTypeNode typeName, String kind) {
        this.typeName=new TypeNode(new BasicTypeWrappedNode(typeName));
        this.varName=varName;
        this.kind=kind;
    }
    public LocalVarSymbolTableRow(String varName, BasicTypeWrappedNode typeName, String kind) {
        this.typeName=new TypeNode(typeName);
        this.varName=varName;
        this.kind=kind;
    }

    @Override
    public String toString(){

        return String.format("| %8s | %8s | %8s |",varName,typeName,kind);
    }

    public String getName() {
        return this.varName;
    }

    public TypeNode getType() {
        return this.typeName;
    }

    public TypeNode getReturnTypeIfIsSubroutine()throws Exception{
        if(this.typeName.type instanceof BasicTypeWrappedNode){
            BasicTypeWrappedNode t1 = (BasicTypeWrappedNode)this.typeName.type;
            if(t1.typenode instanceof SubroutineTypeNode){
                return ((SubroutineTypeNode) t1.typenode).returnType;
            }
        }

        throw new Exception(this.getClass().getSimpleName()
                +": tried to get return type of subroutine variable "
                +this.varName+" but it is not a subroutine variable. its type is "
                +this.getType());

    }

    /**there may be 2 local variables and 2 local argument variables in a subroutine.
     * so there are
     * LOCAL 0,LOCAL 1,
     * and,
     * ARG 0, ARG 1
     * @return returns the index of the local variable in it's segment.
     */

}
