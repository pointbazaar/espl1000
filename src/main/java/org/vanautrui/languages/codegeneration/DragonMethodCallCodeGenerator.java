package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.DragonMethodScopeSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonMethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

import static org.objectweb.asm.Opcodes.GETSTATIC;
import static org.objectweb.asm.Opcodes.INVOKEVIRTUAL;

public class DragonMethodCallCodeGenerator {

    public static void visitMethodCallNode(ClassWriter cw, MethodVisitor mv, DragonClassNode classNode, DragonMethodNode methodNode, DragonMethodCallNode methodCallNode, DragonMethodScopeSymbolTable methodScopeSymbolTable) throws Exception {
        //TODO: actually compile the stuff, not just fake

        mv.visitFieldInsn(GETSTATIC,
                "java/lang/System",
                "out",
                "Ljava/io/PrintStream;");

        if(methodCallNode.argumentList.size()>0) {
            //mv.visitLdcInsn(methodCallNode.argumentList.get(0).str);
            for(DragonExpressionNode expressionNode : methodCallNode.argumentList){
                DragonExpressionCodeGenerator.visitExpression(cw,mv,classNode,methodNode,expressionNode,methodScopeSymbolTable);
            }
            //DragonStringConstantCodeGenerator.visitStringConstant(cw,mv,classNode,methodNode,methodCallNode.argumentList.get(0),methodScopeSymbolTable);
        }else{
            mv.visitLdcInsn("");
        }

        switch (methodCallNode.identifierMethodName.name.getContents()) {
            case "println":
                mv.visitMethodInsn(INVOKEVIRTUAL,
                        "java/io/PrintStream",
                        "println",
                        "(Ljava/lang/String;)V");
                break;
            case "print":
                mv.visitMethodInsn(INVOKEVIRTUAL,
                        "java/io/PrintStream",
                        "print",
                        "(Ljava/lang/String;)V");
                break;
            default:
                throw new Exception("unrecognized method "+methodCallNode.identifierMethodName.name.getContents());
        }
    }
}
