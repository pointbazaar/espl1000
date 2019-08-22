package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.symboltables.tables.LocalVarSymbolTable;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.typeresolution.TypeResolver;

import static org.objectweb.asm.Opcodes.*;

public class ReturnStatementCodeGenerator {
    public static void visitReturnStatement(ClassWriter cw, MethodVisitor mv,
                                            ClassNode classNode, MethodNode methodNode,
                                            ReturnStatementNode returnStatementNode,
                                            LocalVarSymbolTable methodScopeSymbolTable,
                                            SubroutineSymbolTable subroutineSymbolTable, boolean debug) throws Exception{

        if(returnStatementNode.returnValue.isPresent()){
            //put the expression on the stack
            ExpressionCodeGenerator.visitExpression(cw,mv,classNode,methodNode,returnStatementNode.returnValue.get(),methodScopeSymbolTable,subroutineSymbolTable,debug);

            //determine the return type
            //TODO: consider the other return types
            String returnValueType= TypeResolver.getTypeExpressionNode(returnStatementNode.returnValue.get(),methodNode,subroutineSymbolTable,methodScopeSymbolTable);
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
