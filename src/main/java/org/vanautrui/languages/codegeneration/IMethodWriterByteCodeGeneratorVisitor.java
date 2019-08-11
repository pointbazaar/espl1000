package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonMethodNode;

import java.util.Optional;

public interface IMethodWriterByteCodeGeneratorVisitor {

    public void visit(MethodVisitor mv, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod);
    //this interface can be implemented
    //by Statements and everything below
    //in the abstract syntax tree.
    //It is supposed to facilitate
    //writing the bytecode in order to compile the language
}
