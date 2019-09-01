package org.vanautrui.languages.codegeneration.jvmbackend;

import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.terminal.StringConstantNode;

public class StringConstantCodeGenerator {

    public static void visitStringConstant(
            MethodVisitor mv,
            StringConstantNode stringConstantNode
    ) throws Exception {
        //load constant
        mv.visitLdcInsn(stringConstantNode.str);
    }
}
