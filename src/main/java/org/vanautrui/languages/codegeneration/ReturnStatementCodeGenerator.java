package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.Label;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.symboltables.tables.LocalVarSymbolTable;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;
import org.vanautrui.languages.typeresolution.TypeResolver;

import static org.objectweb.asm.Opcodes.*;

public class ReturnStatementCodeGenerator {
    public static void visitReturnStatement(ClassWriter cw, MethodVisitor mv,
                                            ClassNode classNode, MethodNode methodNode,
                                            ReturnStatementNode returnStatementNode,
                                            LocalVarSymbolTable methodScopeSymbolTable,
                                            SubroutineSymbolTable subroutineSymbolTable, boolean debug) throws Exception{

        //put the expression on the stack
        mv.visitLabel(new Label()); //i think it should not be neccessary

        ExpressionCodeGenerator.visitExpression(cw,mv,classNode,methodNode,returnStatementNode.returnValue,methodScopeSymbolTable,subroutineSymbolTable,debug);

        if(methodNode.methodName.methodName.name.equals("main")){
            //main is Void in java, but we do not have 'Void' in this language

            //also consider the exit code
            String methodDescriptor ="(I)V";
            mv.visitMethodInsn(INVOKESTATIC,
                    "java/lang/System",
                    "exit",
                    methodDescriptor);

            mv.visitLabel(new Label());//i think it should not be neccessary
            mv.visitInsn(RETURN);
        }else{


            //determine the return type
            //TODO: consider the other return types
            String returnValueType= TypeResolver.getTypeExpressionNode(returnStatementNode.returnValue,methodNode,subroutineSymbolTable,methodScopeSymbolTable);
            switch (returnValueType){
                case "Int":
                    mv.visitInsn(IRETURN);
                    break;
                case "String":
                case "[Int]":
                case "[Float]":
                case "[Bool]":
                    mv.visitInsn(ARETURN);
                    break;
                case "Float":
                    mv.visitInsn(FRETURN);
                    break;
                default:
                    throw new Exception("could not determine which return to use (ReturnStatementCodeGenerator)");
            }
        }
    }
}
