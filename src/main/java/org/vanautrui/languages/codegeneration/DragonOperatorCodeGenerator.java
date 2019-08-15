package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonOperatorNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonMethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

import static org.objectweb.asm.Opcodes.*;

public class DragonOperatorCodeGenerator {

    public static void visitOperatorNode(ClassWriter cw, MethodVisitor mv, DragonClassNode classNode, DragonMethodNode methodNode, DragonOperatorNode operatorNode) throws Exception {

        switch (operatorNode.operator){
            case "+":
                mv.visitInsn(IADD);
                break;
            case "-":
                mv.visitInsn(ISUB);
                break;
            case "/":
                mv.visitInsn(IDIV);
                break;
            case "*":
                mv.visitInsn(IMUL);
                break;
            default:
                throw new Exception("unhandled case in DragonOperatorCodeGenerator : operator is "+operatorNode.operator);
        }

    }
}
