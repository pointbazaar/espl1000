package org.vanautrui.languages.codegeneration.jvmbackend;

import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.terminal.StringConstNode;

public class StringConstantCodeGenerator {

    public static void visitStringConstant(
            MethodVisitor mv,
            StringConstNode stringConstNode
    ) throws Exception {
        //load constant
        mv.visitLdcInsn(stringConstNode.str);
    }
}
