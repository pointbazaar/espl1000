package org.vanautrui.languages.symboltables.rows;

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

public interface ISymbolTableRow {

    public String getName();
    public String getType();
}
