package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.DragonMethodScopeSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonTermNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIntegerConstantNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonStringConstantNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonVariableNode;

import static org.objectweb.asm.Opcodes.*;
import static org.vanautrui.languages.codegeneration.JavaByteCodeGenerator.pushIntegerConstant;

public class DragonTermCodeGenerator {

    public static void visitTerm(
            ClassWriter cw,
            MethodVisitor mv,
            DragonClassNode classNode,
            DragonMethodNode methodNode,
            DragonTermNode termNode,
            DragonMethodScopeSymbolTable methodScopeSymbolTable
    ) throws Exception {

        if(termNode.termNode instanceof DragonIntegerConstantNode){
            DragonIntegerConstantNode integerConstantNode = (DragonIntegerConstantNode)termNode.termNode;

            pushIntegerConstant(integerConstantNode.value,mv);
            //mv.visitVarInsn(BIPUSH,integerConstantNode.value);

        }else if(termNode.termNode instanceof DragonStringConstantNode){
            DragonStringConstantNode stringConstantNode = (DragonStringConstantNode)termNode.termNode;
            //push the string on the stack
            DragonStringConstantCodeGenerator.visitStringConstant(cw,mv,classNode,methodNode,stringConstantNode,methodScopeSymbolTable);
        }else if(termNode.termNode instanceof DragonExpressionNode) {
            DragonExpressionNode dragonExpressionNode = (DragonExpressionNode)termNode.termNode;
            DragonExpressionCodeGenerator.visitExpression(cw, mv, classNode, methodNode, dragonExpressionNode, methodScopeSymbolTable);
        }else if(termNode.termNode instanceof DragonVariableNode){

            //TODO: find the local variable index and push it onto the stack

            DragonVariableNode variableNode = (DragonVariableNode)termNode.termNode;
            if(methodScopeSymbolTable.containsVariable(variableNode.name.getContents())){
                mv.visitIntInsn(ILOAD,methodScopeSymbolTable.getIndexOfVariable(variableNode.name.getContents()));
            }else{
                throw new Exception("variable "+variableNode.name.getContents()+" not defined?");
            }
        }else{
            throw new Exception("unhandled case in DragonTermCodeGenerator.java");
        }
    }
}
