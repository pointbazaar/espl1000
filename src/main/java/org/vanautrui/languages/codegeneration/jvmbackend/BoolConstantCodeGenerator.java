package org.vanautrui.languages.codegeneration.jvmbackend;

import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.terminal.BoolConstNode;

import static org.vanautrui.languages.codegeneration.jvmbackend.IntegerConstantCodeGenerator.pushIntegerConstant;

public class BoolConstantCodeGenerator {

    public static void visitBoolConstant(
            MethodVisitor mv,
            BoolConstNode boolConstNode
    ) throws Exception {
        pushIntegerConstant((boolConstNode.value)?1:0,mv);
    }
}
