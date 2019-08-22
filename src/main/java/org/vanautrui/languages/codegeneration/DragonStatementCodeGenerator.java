package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonMethodScopeVariableSymbolTable;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonSubroutineSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonAssignmentStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonMethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonIfStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonLoopStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonReturnStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonWhileStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.typeresolution.DragonTypeResolver;

import static org.objectweb.asm.Opcodes.*;
import static org.vanautrui.languages.codegeneration.DragonLoopStatementCodeGenerator.visitLoopStatmentNode;

public class DragonStatementCodeGenerator {

    public static void visitStatement(
            ClassWriter cw,
            MethodVisitor mv,
            DragonClassNode classNode,
            DragonMethodNode methodNode,
            DragonStatementNode statementNode,
            DragonSubroutineSymbolTable subroutineSymbolTable,
            DragonMethodScopeVariableSymbolTable methodScopeSymbolTable,
            boolean debug
    ) throws Exception {

        //TODO: consider other statement types and such
        //statementNode.methodCallNode.visit(mv,classNode,methodNode);
        if(statementNode.statementNode instanceof DragonMethodCallNode){
            DragonMethodCallNode call = (DragonMethodCallNode)statementNode.statementNode;
            DragonMethodCallCodeGenerator.visitMethodCallNode(cw,mv,classNode,methodNode,call,methodScopeSymbolTable,subroutineSymbolTable,debug);

            //TODO: if the method was not Void, we should pop something off the stack
            String returnType = DragonTypeResolver.getTypeMethodCallNode(call,subroutineSymbolTable);

            if(!returnType.equals("Void")){
                mv.visitInsn(POP);
            }
        }else if(statementNode.statementNode instanceof DragonLoopStatementNode) {
            DragonLoopStatementNode loop = (DragonLoopStatementNode) statementNode.statementNode;
            visitLoopStatmentNode(cw, mv, classNode, methodNode, loop,methodScopeSymbolTable,subroutineSymbolTable,debug);
        }else if(statementNode.statementNode instanceof DragonAssignmentStatementNode) {

            DragonAssignmentStatementNode assignmentStatementNode = (DragonAssignmentStatementNode) statementNode.statementNode;

            int local_var_index = methodScopeSymbolTable.getIndexOfVariable(assignmentStatementNode.variableNode.name.getContents());

            //evaluate the expression and store the result in the local variable
            DragonExpressionCodeGenerator
                    .visitExpression(cw, mv, classNode, methodNode, assignmentStatementNode.expressionNode, methodScopeSymbolTable,subroutineSymbolTable,debug);

            String expressionType= DragonTypeResolver.getTypeExpressionNode(assignmentStatementNode.expressionNode,methodNode,subroutineSymbolTable,methodScopeSymbolTable);
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
                default:
                    throw new Exception("unconsidered case in DragonStatementCodeGenerator. type was :"+expressionType);
            }
        }else if(statementNode.statementNode instanceof DragonWhileStatementNode){
            DragonWhileStatementNode whileStatementNode =(DragonWhileStatementNode)statementNode.statementNode;
            DragonWhileStatementCodeGenerator.visitWhileStatmentNode(cw,mv,classNode,methodNode,whileStatementNode,methodScopeSymbolTable,subroutineSymbolTable,debug);
        }else if(statementNode.statementNode instanceof DragonIfStatementNode) {
            DragonIfStatementNode ifStatementNode = (DragonIfStatementNode) statementNode.statementNode;
            DragonIfStatementCodeGenerator.visitIfStatmentNode(cw, mv, classNode, methodNode, ifStatementNode, methodScopeSymbolTable, subroutineSymbolTable,debug);
        }else if(statementNode.statementNode instanceof DragonReturnStatementNode){
            DragonReturnStatementNode returnStatementNode = (DragonReturnStatementNode)statementNode.statementNode;
            DragonReturnStatementCodeGenerator.visitReturnStatement(cw,mv,classNode,methodNode,returnStatementNode,methodScopeSymbolTable,subroutineSymbolTable,debug);
        }else{
            throw new Exception("unconsidered statement type: "+statementNode.statementNode.getClass().getName());
        }
    }
}
