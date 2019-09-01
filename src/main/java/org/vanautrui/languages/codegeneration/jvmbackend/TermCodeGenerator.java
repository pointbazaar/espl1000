package org.vanautrui.languages.codegeneration.jvmbackend;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.*;
import org.vanautrui.languages.symboltables.tables.LocalVarSymbolTable;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;

import static org.vanautrui.languages.codegeneration.jvmbackend.IntegerConstantCodeGenerator.pushIntegerConstant;
import static org.vanautrui.languages.codegeneration.jvmbackend.JavaByteCodeGenerator.pushFloatConstant;

public class TermCodeGenerator {

    public static void visitTerm(
            ClassWriter cw,
            MethodVisitor mv,
            ClassNode classNode,
            MethodNode methodNode,
            TermNode termNode,
            LocalVarSymbolTable varTable,
            SubroutineSymbolTable subTable,
            boolean debug
    ) throws Exception {
	if(termNode.termNode instanceof FloatConstantNode){
		FloatConstantNode node=(FloatConstantNode)termNode.termNode;
		pushFloatConstant(node.value,mv);
	}else if(termNode.termNode instanceof IntegerConstantNode){
            IntegerConstantNode integerConstantNode = (IntegerConstantNode)termNode.termNode;
            pushIntegerConstant(integerConstantNode.value,mv);
        }else if(termNode.termNode instanceof StringConstantNode){
            StringConstantNode stringConstantNode = (StringConstantNode)termNode.termNode;
            //push the string on the stack
            StringConstantCodeGenerator.visitStringConstant(mv,stringConstantNode);
        }else if(termNode.termNode instanceof ExpressionNode) {
            ExpressionNode expressionNode = (ExpressionNode)termNode.termNode;
            ExpressionCodeGenerator.visitExpression(cw, mv, classNode, methodNode, expressionNode, varTable,subTable,debug);
        }else if(termNode.termNode instanceof VariableNode) {
            //find the local variable index and push the variable onto the stack
            VariableNode variableNode = (VariableNode) termNode.termNode;
            VariableCodeGenerator.visitVariableNode(cw,classNode,variableNode,mv,methodNode,varTable,subTable,debug);
        }else if(termNode.termNode instanceof MethodCallNode){
            MethodCallNode methodCallNode = (MethodCallNode)termNode.termNode;
            MethodCallCodeGenerator.visitMethodCallNode(cw,mv,classNode,methodNode,methodCallNode,varTable,subTable,debug);
		}else if(termNode.termNode instanceof BoolConstantNode) {
            BoolConstantNode b = (BoolConstantNode) termNode.termNode;
            BoolConstantCodeGenerator.visitBoolConstant(mv, b);
        }else if(termNode.termNode instanceof ArrayConstantNode) {
        ArrayConstantNode arrayConstantNode = (ArrayConstantNode) termNode.termNode;
        ArrayConstantCodeGenerator.visitArrayConstant(cw, mv, classNode, methodNode, arrayConstantNode, varTable, subTable, debug);
        } else if (termNode.termNode instanceof CharConstantNode) {
            CharConstantCodeGenerator.visitCharConstant(mv,(CharConstantNode)termNode.termNode);
        }else{
            throw new Exception("unhandled case in TermCodeGenerator.java");
        }
    }
}
