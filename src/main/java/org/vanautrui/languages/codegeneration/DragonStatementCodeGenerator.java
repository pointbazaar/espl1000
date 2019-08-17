package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;

import org.vanautrui.languages.codegeneration.symboltables.DragonMethodScopeSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonAssignmentStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonMethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonLoopStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonWhileStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

import static org.objectweb.asm.Opcodes.GETSTATIC;
import static org.objectweb.asm.Opcodes.INVOKEVIRTUAL;
import static org.objectweb.asm.Opcodes.*;
import static org.vanautrui.languages.codegeneration.DragonLoopStatementCodeGenerator.visitLoopStatmentNode;

public class DragonStatementCodeGenerator {

    public static void visitStatement(
            ClassWriter cw,
            MethodVisitor mv,
            DragonClassNode classNode,
            DragonMethodNode methodNode,
            DragonStatementNode statementNode,
            DragonMethodScopeSymbolTable methodScopeSymbolTable
    ) throws Exception {

        //TODO: consider other statement types and such
        //statementNode.methodCallNode.visit(mv,classNode,methodNode);
        if(statementNode.statementNode instanceof DragonMethodCallNode){
            DragonMethodCallNode call = (DragonMethodCallNode)statementNode.statementNode;
            DragonMethodCallCodeGenerator.visitMethodCallNode(cw,mv,classNode,methodNode,call,methodScopeSymbolTable);
        }else if(statementNode.statementNode instanceof DragonLoopStatementNode) {
            DragonLoopStatementNode loop = (DragonLoopStatementNode) statementNode.statementNode;
            visitLoopStatmentNode(cw, mv, classNode, methodNode, loop,methodScopeSymbolTable);
        }else if(statementNode.statementNode instanceof DragonAssignmentStatementNode) {

            DragonAssignmentStatementNode assignmentStatementNode = (DragonAssignmentStatementNode) statementNode.statementNode;

            int local_var_index = methodScopeSymbolTable.getIndexOfVariable(assignmentStatementNode.variableNode.name.getContents());

            //evaluate the expression and store the result in the local variable
            DragonExpressionCodeGenerator
                    .visitExpression(cw, mv, classNode, methodNode, assignmentStatementNode.expressionNode, methodScopeSymbolTable);

            mv.visitVarInsn(ISTORE, local_var_index);
        }else if(statementNode.statementNode instanceof DragonWhileStatementNode){
            DragonWhileStatementNode whileStatementNode =(DragonWhileStatementNode)statementNode.statementNode;
            DragonWhileStatementCodeGenerator.visitWhileStatmentNode(cw,mv,classNode,methodNode,whileStatementNode,methodScopeSymbolTable);

        }else{
            throw new Exception("unconsidered statement type");
        }
    }
}
