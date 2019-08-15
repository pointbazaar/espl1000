package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.DragonMethodScopeSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonOperatorNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonTermNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonAssignmentStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonMethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonLoopStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

import static org.objectweb.asm.Opcodes.ISTORE;

public class DragonExpressionCodeGenerator {

    public static void visitExpression(
            ClassWriter cw,
            MethodVisitor mv,
            DragonClassNode classNode,
            DragonMethodNode methodNode,
            DragonExpressionNode expressionNode,
            DragonMethodScopeSymbolTable methodScopeSymbolTable
    ) throws Exception {

        //evaluate the expression
        DragonTermCodeGenerator.visitTerm(cw,mv,classNode,methodNode,expressionNode.term,methodScopeSymbolTable);

        for(int i=0;i<expressionNode.termNodes.size();i++){

            DragonTermNode myterm=expressionNode.termNodes.get(i);

            DragonTermCodeGenerator.visitTerm(cw,mv,classNode,methodNode,myterm,methodScopeSymbolTable);

            DragonOperatorNode myop=expressionNode.operatorNodes.get(i);

            DragonOperatorCodeGenerator.visitOperatorNode(cw,mv,classNode,methodNode,myop);
        }
    }
}
