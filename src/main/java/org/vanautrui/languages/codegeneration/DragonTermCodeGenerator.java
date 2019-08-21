package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonMethodScopeVariableSymbolTable;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonSubroutineSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonTermNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonMethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.*;

import static org.objectweb.asm.Opcodes.*;
import static org.vanautrui.languages.codegeneration.JavaByteCodeGenerator.*;

public class DragonTermCodeGenerator {

    public static void visitTerm(
            ClassWriter cw,
            MethodVisitor mv,
            DragonClassNode classNode,
            DragonMethodNode methodNode,
            DragonTermNode termNode,
            DragonMethodScopeVariableSymbolTable methodScopeSymbolTable,
            DragonSubroutineSymbolTable subroutineSymbolTable,
            boolean debug
    ) throws Exception {
	if(termNode.termNode instanceof DragonFloatConstantNode){
		DragonFloatConstantNode node=(DragonFloatConstantNode)termNode.termNode;
		pushFloatConstant(node.value,mv);
	}else if(termNode.termNode instanceof DragonIntegerConstantNode){
            DragonIntegerConstantNode integerConstantNode = (DragonIntegerConstantNode)termNode.termNode;

            pushIntegerConstant(integerConstantNode.value,mv);
        }else if(termNode.termNode instanceof DragonStringConstantNode){
            DragonStringConstantNode stringConstantNode = (DragonStringConstantNode)termNode.termNode;
            //push the string on the stack
            DragonStringConstantCodeGenerator.visitStringConstant(mv,stringConstantNode);
        }else if(termNode.termNode instanceof DragonExpressionNode) {
            DragonExpressionNode dragonExpressionNode = (DragonExpressionNode)termNode.termNode;
            DragonExpressionCodeGenerator.visitExpression(cw, mv, classNode, methodNode, dragonExpressionNode, methodScopeSymbolTable,subroutineSymbolTable,debug);
        }else if(termNode.termNode instanceof DragonVariableNode) {

            //TODO: find the local variable index and push it onto the stack

            DragonVariableNode variableNode = (DragonVariableNode) termNode.termNode;
            if (methodScopeSymbolTable.containsVariable(variableNode.name.getContents())) {
                String type = methodScopeSymbolTable.getTypeOfVariable(variableNode.name.getContents());
                switch (type) {
                    case "Int":
                        mv.visitIntInsn(ILOAD, methodScopeSymbolTable.getIndexOfVariable(variableNode.name.getContents()));
                        break;
			case "Float":
				mv.visitIntInsn(FLOAD,methodScopeSymbolTable.getIndexOfVariable(variableNode.name.getContents()));
				break;
                    case "String":
                        mv.visitIntInsn(ALOAD,methodScopeSymbolTable.getIndexOfVariable(variableNode.name.getContents()));
                        break;
                    default:
                        throw new Exception("unconsidered case in DragonTermCodeGenerator: type:"+type);
                }
            } else {
                throw new Exception("variable " + variableNode.name.getContents() + " not defined?");
            }
        }else if(termNode.termNode instanceof DragonMethodCallNode){
            DragonMethodCallNode methodCallNode = (DragonMethodCallNode)termNode.termNode;

            DragonMethodCallCodeGenerator.visitMethodCallNode(cw,mv,classNode,methodNode,methodCallNode,methodScopeSymbolTable,subroutineSymbolTable,debug);
        }else{
            throw new Exception("unhandled case in DragonTermCodeGenerator.java");
        }
    }
}
