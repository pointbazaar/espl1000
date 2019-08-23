package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.symboltables.tables.LocalVarSymbolTable;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.*;

import static org.objectweb.asm.Opcodes.*;
import static org.vanautrui.languages.codegeneration.IntegerConstantCodeGenerator.pushIntegerConstant;
import static org.vanautrui.languages.codegeneration.JavaByteCodeGenerator.*;

public class TermCodeGenerator {

    public static void visitTerm(
            ClassWriter cw,
            MethodVisitor mv,
            ClassNode classNode,
            MethodNode methodNode,
            TermNode termNode,
            LocalVarSymbolTable methodScopeSymbolTable,
            SubroutineSymbolTable subroutineSymbolTable,
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
            ExpressionCodeGenerator.visitExpression(cw, mv, classNode, methodNode, expressionNode, methodScopeSymbolTable,subroutineSymbolTable,debug);
        }else if(termNode.termNode instanceof VariableNode) {

            //TODO: find the local variable index and push it onto the stack

            VariableNode variableNode = (VariableNode) termNode.termNode;
            if (methodScopeSymbolTable.containsVariable(variableNode.name)) {
                String type = methodScopeSymbolTable.getTypeOfVariable(variableNode.name);
                switch (type) {
                    case "Int":
					case "Bool":
                        mv.visitIntInsn(
							ILOAD, 
							methodScopeSymbolTable.getIndexOfVariable(
								variableNode.name
							)
						);
                        break;
					case "Float":
						mv.visitIntInsn(
							FLOAD,
							methodScopeSymbolTable.getIndexOfVariable(variableNode.name)
						);
						break;
                    case "String":
                        mv.visitIntInsn(ALOAD,methodScopeSymbolTable.getIndexOfVariable(variableNode.name));
                        break;
                    default:
                        throw new Exception("unconsidered case in DragonTermCodeGenerator: type:"+type);
                }
            } else {
                throw new Exception("variable " + variableNode.name + " not defined?");
            }
        }else if(termNode.termNode instanceof MethodCallNode){
            MethodCallNode methodCallNode = (MethodCallNode)termNode.termNode;

            MethodCallCodeGenerator.visitMethodCallNode(cw,mv,classNode,methodNode,methodCallNode,methodScopeSymbolTable,subroutineSymbolTable,debug);
		}else if(termNode.termNode instanceof BoolConstantNode) {
            BoolConstantNode b = (BoolConstantNode) termNode.termNode;
            BoolConstantCodeGenerator.visitBoolConstant(mv, b);
        }else if(termNode.termNode instanceof ArrayConstantNode){
	        ArrayConstantNode arrayConstantNode = (ArrayConstantNode)termNode.termNode;
            ArrayConstantCodeGenerator.visitArrayConstant(cw,mv,classNode,methodNode,arrayConstantNode,methodScopeSymbolTable,subroutineSymbolTable,debug);
        }else{
            throw new Exception("unhandled case in DragonTermCodeGenerator.java");
        }
    }
}
