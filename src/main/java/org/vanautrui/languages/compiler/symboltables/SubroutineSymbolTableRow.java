package org.vanautrui.languages.compiler.symboltables;

import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;

public class SubroutineSymbolTableRow {

    private final String subRoutineName;
    //almost all subroutines have a name, except anonymous subroutines or anonymous lambdas
    //but we can make up names for these maybe

    private final TypeNode typeName;  //every subroutine has a return type
    private final String className; //every subroutine is contained in a class

    private final int numberOfLocalVariables;
    private final int numberOfArguments;
    public SubroutineSymbolTableRow(String subRoutineName, IBasicAndWrappedTypeNode typeName, String className, int numberOfLocalVariables, int nArgs){

        this.typeName=new TypeNode(new BasicTypeWrappedNode(typeName));
        this.subRoutineName = subRoutineName;
        this.className=className;
        this.numberOfLocalVariables=numberOfLocalVariables;
        this.numberOfArguments=nArgs;
    }
    public SubroutineSymbolTableRow(String subRoutineName, TypeNode typeName, String className, int numberOfLocalVariables, int nArgs){

        this.typeName=typeName;
        this.subRoutineName = subRoutineName;
        this.className=className;
        this.numberOfLocalVariables=numberOfLocalVariables;
        this.numberOfArguments=nArgs;
    }

    @Override
    public String toString(){

        return String.format("| %8s | %8s | %8s |", subRoutineName,typeName,className);
    }

    public String getName() {
        return this.subRoutineName;
    }

    public TypeNode getType() {
        return this.typeName;
    }

    public String getClassName(){return this.className;}

    public int getNumberOfLocalVariables() {
        return this.numberOfLocalVariables;
    }

    public int getNumberOfArguments() {
        return this.numberOfArguments;
    }

    public static String generateVMCodeSubroutineName(String className,String subRoutineName){
        if(subRoutineName.equals("main")){
            return "Main_main"; //main must be the same, even if we are in a different class
        }else {
            return className + "_" + subRoutineName;
        }
    }
}


/*
if there was only a way to constrain visibility of
classes and interfaces in a more expressive way...

in java afaik you can only have:
    private, package,procected, public

but what if i want to have a class or interface
visible to the whole package that is 3 levels above me?

package1
|- package1.1
|-- MyClass3
|-- package1.1.1
|--- MyClass
|- package1.2
|-- MyClass2


Let us say i want MyClass to be visible to MyClass2.

But i want it to be hidden from MyClass3.

there should be a mechanism in the Dragon Language
to achieve this. like for example:

@visible to package1
@hidden to package1.1
 */