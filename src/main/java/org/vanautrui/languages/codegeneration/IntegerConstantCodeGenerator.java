package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.terminal.BoolConstantNode;

import static org.objectweb.asm.Opcodes.*;

public class IntegerConstantCodeGenerator {

    public static void pushIntegerConstant(int n, MethodVisitor mv){
        switch (n){
            case 0:
                mv.visitInsn(ICONST_0);
                break;
            case 1:
                mv.visitInsn(ICONST_1);
                break;
            case 2:
                mv.visitInsn(ICONST_2);
                break;
            case 3:
                mv.visitInsn(ICONST_3);
                break;
            case 4:
                mv.visitInsn(ICONST_4);
                break;
            case 5:
                mv.visitInsn(ICONST_5);
                break;
            default:
                mv.visitIntInsn(BIPUSH,n);
                break;
        }
    }
}
