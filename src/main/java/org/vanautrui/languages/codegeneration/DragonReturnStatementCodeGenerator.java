package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonMethodScopeVariableSymbolTable;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonSubroutineSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonReturnStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.typeresolution.DragonTypeResolver;

import static org.objectweb.asm.Opcodes.*;

public class DragonReturnStatementCodeGenerator {
    public static void visitReturnStatement(ClassWriter cw, MethodVisitor mv, DragonClassNode classNode, DragonMethodNode methodNode, DragonReturnStatementNode returnStatementNode, DragonMethodScopeVariableSymbolTable methodScopeSymbolTable, DragonSubroutineSymbolTable subroutineSymbolTable) throws Exception{

        if(returnStatementNode.returnValue.isPresent()){
            //put the expression on the stack
            DragonExpressionCodeGenerator.visitExpression(cw,mv,classNode,methodNode,returnStatementNode.returnValue.get(),methodScopeSymbolTable,subroutineSymbolTable);

            //determine the return type
            //TODO: consider the other return types
            String returnValueType= DragonTypeResolver.getTypeExpressionNode(returnStatementNode.returnValue.get(),methodNode,subroutineSymbolTable);
            switch (returnValueType){
                case "Int":
                    mv.visitInsn(IRETURN);
                    break;
                case "String":
                    mv.visitInsn(ARETURN);
                    break;
                default:
                    throw new Exception("could not determine which return to use (DragonReturnStatementCodeGenerator)");
            }
        }else {
            //return control to the caller
            mv.visitInsn(RETURN);
        }
    }
}
