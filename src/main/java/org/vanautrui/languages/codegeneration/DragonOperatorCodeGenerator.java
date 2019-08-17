package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.Label;
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
            case "<":
            case ">":
            case "==":
                //if less than, push 1
                //else push 0

                Label truthCaseLabel = new Label();
                Label endLabel = new Label();

                if(operatorNode.operator.equals("<")) {
                    mv.visitJumpInsn(IF_ICMPLT, truthCaseLabel);
                }else if(operatorNode.operator.equals(">")){
                    mv.visitJumpInsn(IF_ICMPGT, truthCaseLabel);
                }else if(operatorNode.operator.equals("==")){
                    mv.visitJumpInsn(IF_ICMPEQ,truthCaseLabel);
                }

                mv.visitInsn(ICONST_0);
                mv.visitJumpInsn(GOTO,endLabel);


                mv.visitLabel(truthCaseLabel);
                mv.visitInsn(ICONST_1);
                mv.visitJumpInsn(GOTO,endLabel);

                mv.visitLabel(endLabel);
                break;
            default:
                throw new Exception("unhandled case in DragonOperatorCodeGenerator : operator is "+operatorNode.operator);
        }

    }
}
