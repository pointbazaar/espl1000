package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonMethodScopeVariableSymbolTable;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonSubroutineSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonOperatorNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonTermNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import static org.vanautrui.languages.typeresolution.DragonTypeResolver.*;
public class ExpressionCodeGenerator {

    public static void visitExpression(
            ClassWriter cw,
            MethodVisitor mv,
            DragonClassNode classNode,
            DragonMethodNode methodNode,
            DragonExpressionNode expressionNode,
            DragonMethodScopeVariableSymbolTable methodScopeSymbolTable,
            DragonSubroutineSymbolTable subroutineSymbolTable,
            boolean debug
    ) throws Exception {

        //evaluate the expression
        TermCodeGenerator.visitTerm(cw,mv,classNode,methodNode,expressionNode.term,methodScopeSymbolTable,subroutineSymbolTable,debug);

        for(int i=0;i<expressionNode.termNodes.size();i++){

            DragonTermNode myterm=expressionNode.termNodes.get(i);

            TermCodeGenerator.visitTerm(cw,mv,classNode,methodNode,myterm,methodScopeSymbolTable,subroutineSymbolTable,debug);

		String operandType=getTypeTermNode(myterm,methodNode,subroutineSymbolTable,methodScopeSymbolTable);
            DragonOperatorNode myop=expressionNode.operatorNodes.get(i);

            OperatorCodeGenerator.visitOperatorNode(cw,mv,classNode,methodNode,myop,operandType);
        }
    }
}
