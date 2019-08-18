package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonMethodScopeVariableSymbolTable;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonSubroutineSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonReturnStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import static org.objectweb.asm.Opcodes.*;

public class DragonReturnStatementCodeGenerator {
    public static void visitReturnStatement(ClassWriter cw, MethodVisitor mv, DragonClassNode classNode, DragonMethodNode methodNode, DragonReturnStatementNode returnStatementNode, DragonMethodScopeVariableSymbolTable methodScopeSymbolTable, DragonSubroutineSymbolTable subroutineSymbolTable) throws Exception{

        if(returnStatementNode.returnValue.isPresent()){
            //put the expression on the stack
            DragonExpressionCodeGenerator.visitExpression(cw,mv,classNode,methodNode,returnStatementNode.returnValue.get(),methodScopeSymbolTable,subroutineSymbolTable);
            mv.visitInsn(ARETURN);
        }else {
            //return control to the caller
            mv.visitInsn(RETURN);
        }
    }
}
