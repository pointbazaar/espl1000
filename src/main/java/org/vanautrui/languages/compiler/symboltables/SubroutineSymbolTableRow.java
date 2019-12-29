package org.vanautrui.languages.compiler.symboltables;

import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SubroutineTypeNode;

import java.util.List;

public final class SubroutineSymbolTableRow {

    private final String subRoutineName;
    //almost all subroutines have a name, except anonymous subroutines or anonymous lambdas
    //but we can make up names for these maybe

    private final String className; //every subroutine is contained in a class

    private final int numberOfLocalVariables;

    private final SubroutineTypeNode type;  //contains return type, argument types

    public SubroutineSymbolTableRow(
            final String subRoutineName,
            final TypeNode returnTypeName,
            final String className,
            final int numberOfLocalVariables,
            final List<TypeNode> arg_types,
            final boolean hasSideEffects
    ){

        this.subRoutineName = subRoutineName;
        this.className=className;
        this.numberOfLocalVariables=numberOfLocalVariables;
        this.type = new SubroutineTypeNode(arg_types,returnTypeName,hasSideEffects);
    }

    public SubroutineSymbolTableRow(
            final String subRoutineName,
            final String className,
            final int numberOfLocalVariables,
            SubroutineTypeNode type){

        this.subRoutineName = subRoutineName;
        this.className=className;
        this.numberOfLocalVariables=numberOfLocalVariables;
        this.type = type;

    }

    @Override
    public synchronized String toString(){

        return String.format("| %8s | %8s | %8s |", subRoutineName, type.returnType,className);
    }

    public synchronized String getName() {
        return this.subRoutineName;
    }

    public synchronized TypeNode getReturnType() {
        return this.type.returnType;
    }

    public synchronized List<TypeNode> getArgumentTypes(){
        return this.type.argumentTypes;
    }

    public synchronized String getClassName(){return this.className;}

    public synchronized int getNumberOfLocalVariables() {
        return this.numberOfLocalVariables;
    }

    public synchronized int getNumberOfArguments() {
        return this.type.argumentTypes.size();
    }

    public synchronized static String generateVMCodeSubroutineName(
            final String className,
            final String subRoutineName
    ){
        if(subRoutineName.equals("main")){
            return "Main_main"; //main must be the same, even if we are in a different class
        }else {
            return className + "_" + subRoutineName;
        }
    }

    public TypeNode getType() {
        return new TypeNode(new BasicTypeWrappedNode(this.type));
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