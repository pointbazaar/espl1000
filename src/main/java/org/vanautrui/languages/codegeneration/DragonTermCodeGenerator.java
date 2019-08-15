package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.DragonMethodScopeSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonTermNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

import static org.objectweb.asm.Opcodes.*;

public class DragonTermCodeGenerator {

    public static void visitTerm(
            ClassWriter cw,
            MethodVisitor mv,
            DragonClassNode classNode,
            DragonMethodNode methodNode,
            DragonTermNode termNode,
            DragonMethodScopeSymbolTable methodScopeSymbolTable
    ) throws Exception {

        if(termNode.integerConstantNode.isPresent()){
            mv.visitVarInsn(BIPUSH,(int)termNode.integerConstantNode.get().value);
        }else if(termNode.stringConstantNode.isPresent()){
            //push the string on the stack
            DragonStringConstantCodeGenerator.visitStringConstant(cw,mv,classNode,methodNode,termNode.stringConstantNode.get(),methodScopeSymbolTable);
        }else if(termNode.expressionNode.isPresent()){
            DragonExpressionCodeGenerator.visitExpression(cw,mv,classNode,methodNode,termNode.expressionNode.get(),methodScopeSymbolTable);
        }else{
            throw new Exception("unhandled case in DragonTermCodeGenerator.java");
        }
    }
}
