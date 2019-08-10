package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;

public interface IClassWriterByteCodeGeneratorVisitor {

    public void visit(ClassWriter classWriter);
    //this interface can be implemented
    //by ClassNodes and everything below
    //in the abstract syntax tree.
    //It is supposed to facilitate
    //writing the bytecode in order to compile the language
}
