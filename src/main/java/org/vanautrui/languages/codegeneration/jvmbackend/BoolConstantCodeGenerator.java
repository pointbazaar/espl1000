package org.vanautrui.languages.codegeneration.jvmbackend;

import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.terminal.BoolConstantNode;

import static org.vanautrui.languages.codegeneration.jvmbackend.IntegerConstantCodeGenerator.pushIntegerConstant;

public class BoolConstantCodeGenerator {

    public static void visitBoolConstant(
            MethodVisitor mv,
            BoolConstantNode boolConstantNode
    ) throws Exception {
        pushIntegerConstant((boolConstantNode.value)?1:0,mv);
    }
}
