package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.symboltables.tables.LocalVarSymbolTable;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.typeresolution.TypeResolver;

import static org.objectweb.asm.Opcodes.*;
import static org.vanautrui.languages.codegeneration.LoopStatementCodeGenerator.visitLoopStatmentNode;

public class StatementCodeGenerator {

    public static void visitStatement(
            ClassWriter cw,
            MethodVisitor mv,
            ClassNode classNode,
            MethodNode methodNode,
            StatementNode statementNode,
            SubroutineSymbolTable subroutineSymbolTable,
            LocalVarSymbolTable methodScopeSymbolTable,
            boolean debug
    ) throws Exception {

        //TODO: consider other statement types and such
        //statementNode.methodCallNode.visit(mv,classNode,methodNode);
        if(statementNode.statementNode instanceof MethodCallNode){
            MethodCallNode call = (MethodCallNode)statementNode.statementNode;
            MethodCallCodeGenerator.visitMethodCallNode(cw,mv,classNode,methodNode,call,methodScopeSymbolTable,subroutineSymbolTable,debug);

            //TODO: if the method was not Void, we should pop something off the stack
            String returnType = TypeResolver.getTypeMethodCallNode(call,subroutineSymbolTable);

            if(!returnType.equals("Void")){
                mv.visitInsn(POP);
            }
        }else if(statementNode.statementNode instanceof LoopStatementNode) {
            LoopStatementNode loop = (LoopStatementNode) statementNode.statementNode;
            visitLoopStatmentNode(cw, mv, classNode, methodNode, loop,methodScopeSymbolTable,subroutineSymbolTable,debug);
        }else if(statementNode.statementNode instanceof AssignmentStatementNode) {

            AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode) statementNode.statementNode;

            int local_var_index = methodScopeSymbolTable.getIndexOfVariable(assignmentStatementNode.variableNode.name);

            //evaluate the expression and store the result in the local variable
            ExpressionCodeGenerator
                    .visitExpression(cw, mv, classNode, methodNode, assignmentStatementNode.expressionNode, methodScopeSymbolTable,subroutineSymbolTable,debug);

            String expressionType= TypeResolver.getTypeExpressionNode(assignmentStatementNode.expressionNode,methodNode,subroutineSymbolTable,methodScopeSymbolTable);
            switch(expressionType){
                case "Int":
				case "Bool":
                    mv.visitVarInsn(ISTORE, local_var_index);
                    break;
				case "Float":
					mv.visitVarInsn(FSTORE, local_var_index);
					break;
                case "String":
                    mv.visitVarInsn(ASTORE,local_var_index);
                    break;
                case "[Int]":
                case "[Float]":
                    mv.visitVarInsn(ASTORE,local_var_index);
                    break;
                default:
                    throw new Exception("unconsidered case in DragonStatementCodeGenerator. type was :"+expressionType);
            }
        }else if(statementNode.statementNode instanceof WhileStatementNode){
            WhileStatementNode whileStatementNode =(WhileStatementNode)statementNode.statementNode;
            WhileStatementCodeGenerator.visitWhileStatmentNode(cw,mv,classNode,methodNode,whileStatementNode,methodScopeSymbolTable,subroutineSymbolTable,debug);
        }else if(statementNode.statementNode instanceof IfStatementNode) {
            IfStatementNode ifStatementNode = (IfStatementNode) statementNode.statementNode;
            IfStatementCodeGenerator.visitIfStatmentNode(cw, mv, classNode, methodNode, ifStatementNode, methodScopeSymbolTable, subroutineSymbolTable,debug);
        }else if(statementNode.statementNode instanceof ReturnStatementNode){
            ReturnStatementNode returnStatementNode = (ReturnStatementNode)statementNode.statementNode;
            ReturnStatementCodeGenerator.visitReturnStatement(cw,mv,classNode,methodNode,returnStatementNode,methodScopeSymbolTable,subroutineSymbolTable,debug);
        }else{
            throw new Exception("unconsidered statement type: "+statementNode.statementNode.getClass().getName());
        }
    }
}
