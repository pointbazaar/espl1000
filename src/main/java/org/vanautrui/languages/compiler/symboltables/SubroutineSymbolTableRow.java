package org.vanautrui.languages.compiler.symboltables;

public class SubroutineSymbolTableRow {

    private final String subRoutineName;
    //almost all subroutines have a name, except anonymous subroutines or anonymous lambdas
    //but we can make up names for these maybe

    private final String typeName;  //every subroutine has a return type
    private final String className; //every subroutine is contained in a class

    public SubroutineSymbolTableRow(String subRoutineName, String typeName,String className){

        this.typeName=typeName;
        this.subRoutineName = subRoutineName;
        this.className=className;
    }

    @Override
    public String toString(){

        return String.format("| %8s | %8s | %8s |", subRoutineName,typeName,className);
    }

    public String getName() {
        return this.subRoutineName;
    }

    public String getType() {
        return this.typeName;
    }

    public String getClassName(){return this.className;}
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