package org.vanautrui.languages.codegeneration.jvmbackend;

import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.terminal.CharConstantNode;

public class CharConstantCodeGenerator {

    public static void visitCharConstant(
            MethodVisitor mv,
            CharConstantNode c
    ) throws Exception {
        //int and char are kind of the same for the jvm, or so i am told
        IntegerConstantCodeGenerator.pushIntegerConstant((int)c.content,mv);
    }
}
