package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonMethodScopeVariableSymbolTable;
import org.vanautrui.languages.codegeneration.symboltables.tables.DragonSubroutineSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.typeresolution.DragonTypeResolver;

import static org.objectweb.asm.Opcodes.*;

public class ReturnStatementCodeGenerator {
    public static void visitReturnStatement(ClassWriter cw, MethodVisitor mv,
                                            ClassNode classNode, MethodNode methodNode,
                                            ReturnStatementNode returnStatementNode,
                                            DragonMethodScopeVariableSymbolTable methodScopeSymbolTable,
                                            DragonSubroutineSymbolTable subroutineSymbolTable, boolean debug) throws Exception{

        if(returnStatementNode.returnValue.isPresent()){
            //put the expression on the stack
            ExpressionCodeGenerator.visitExpression(cw,mv,classNode,methodNode,returnStatementNode.returnValue.get(),methodScopeSymbolTable,subroutineSymbolTable,debug);

            //determine the return type
            //TODO: consider the other return types
            String returnValueType= DragonTypeResolver.getTypeExpressionNode(returnStatementNode.returnValue.get(),methodNode,subroutineSymbolTable,methodScopeSymbolTable);
            switch (returnValueType){
                case "Int":
                    mv.visitInsn(IRETURN);
                    break;
                case "String":
                    mv.visitInsn(ARETURN);
                    break;
		case "Float":
		    mv.visitInsn(FRETURN);
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
