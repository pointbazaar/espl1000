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
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonWhileStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

import static org.objectweb.asm.Opcodes.ASTORE;
import static org.objectweb.asm.Opcodes.ISTORE;
import static org.vanautrui.languages.codegeneration.DragonLoopStatementCodeGenerator.visitLoopStatmentNode;

public class DragonStatementCodeGenerator {

    public static void visitStatement(
            ClassWriter cw,
            MethodVisitor mv,
            DragonClassNode classNode,
            DragonMethodNode methodNode,
            DragonStatementNode statementNode,
            DragonSubroutineSymbolTable subroutineSymbolTable, DragonMethodScopeVariableSymbolTable methodScopeSymbolTable
    ) throws Exception {

        //TODO: consider other statement types and such
        //statementNode.methodCallNode.visit(mv,classNode,methodNode);
        if(statementNode.statementNode instanceof DragonMethodCallNode){
            DragonMethodCallNode call = (DragonMethodCallNode)statementNode.statementNode;
            DragonMethodCallCodeGenerator.visitMethodCallNode(cw,mv,classNode,methodNode,call,methodScopeSymbolTable,subroutineSymbolTable);
        }else if(statementNode.statementNode instanceof DragonLoopStatementNode) {
            DragonLoopStatementNode loop = (DragonLoopStatementNode) statementNode.statementNode;
            visitLoopStatmentNode(cw, mv, classNode, methodNode, loop,methodScopeSymbolTable,subroutineSymbolTable);
        }else if(statementNode.statementNode instanceof DragonAssignmentStatementNode) {

            DragonAssignmentStatementNode assignmentStatementNode = (DragonAssignmentStatementNode) statementNode.statementNode;

            int local_var_index = methodScopeSymbolTable.getIndexOfVariable(assignmentStatementNode.variableNode.name.getContents());

            //evaluate the expression and store the result in the local variable
            DragonExpressionCodeGenerator
                    .visitExpression(cw, mv, classNode, methodNode, assignmentStatementNode.expressionNode, methodScopeSymbolTable,subroutineSymbolTable);

            switch(assignmentStatementNode.expressionNode.getType(methodNode)){
                case "Int":
                    mv.visitVarInsn(ISTORE, local_var_index);
                    break;
                case "String":
                    mv.visitVarInsn(ASTORE,local_var_index);
                    break;
                default:
                    throw new Exception("unconsidered case in DragonStatementCodeGenerator. type was :"+assignmentStatementNode.expressionNode.getType(methodNode));
            }
        }else if(statementNode.statementNode instanceof DragonWhileStatementNode){
            DragonWhileStatementNode whileStatementNode =(DragonWhileStatementNode)statementNode.statementNode;
            DragonWhileStatementCodeGenerator.visitWhileStatmentNode(cw,mv,classNode,methodNode,whileStatementNode,methodScopeSymbolTable,subroutineSymbolTable);
        }else if(statementNode.statementNode instanceof DragonIfStatementNode){
            DragonIfStatementNode ifStatementNode =(DragonIfStatementNode) statementNode.statementNode;
            DragonIfStatementCodeGenerator.visitIfStatmentNode(cw,mv,classNode,methodNode,ifStatementNode,methodScopeSymbolTable,subroutineSymbolTable);
        }else{
            throw new Exception("unconsidered statement type");
        }
    }
}
