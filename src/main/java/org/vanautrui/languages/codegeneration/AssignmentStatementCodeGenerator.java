package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.Label;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.VariableNode;
import org.vanautrui.languages.symboltables.tables.LocalVarSymbolTable;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;
import org.vanautrui.languages.typeresolution.TypeResolver;

import static org.objectweb.asm.Opcodes.*;

public class AssignmentStatementCodeGenerator {

    public static void visitAssignmentStatementNode(
            ClassWriter cw,
            ClassNode classNode,
            AssignmentStatementNode assignmentStatementNode,
             MethodVisitor mv,
             MethodNode methodNode,
             LocalVarSymbolTable varTable,
             SubroutineSymbolTable subTable,
            boolean debug
    ) throws Exception{

        //TODO: consider the type of the value we are assigning to
        //if we are storing into an array we need other bytecodes
        mv.visitLabel(new Label());

        if(assignmentStatementNode.variableNode.indexOptional.isPresent()){
            //assigning into an array at index
            visitAssignVarIntoArray(cw,classNode,assignmentStatementNode,mv,methodNode,varTable,subTable,debug);
        }else{
            visitAssignVarIntoVar(cw,classNode,assignmentStatementNode,mv,methodNode,varTable,subTable,debug);
        }

    }

    private static void visitAssignVarIntoVar(
            ClassWriter cw,
            ClassNode classNode,
            AssignmentStatementNode assignmentStatementNode,
            MethodVisitor mv,
            MethodNode methodNode,
            LocalVarSymbolTable varTable,
            SubroutineSymbolTable subTable,
            boolean debug
    )throws Exception{
        //x=3;y=4;
        //x=y;

        //arr1=arr2

        //both values are primitive, or both are reference types
        int local_var_index = varTable.getIndexOfVariable(assignmentStatementNode.variableNode.name);

        //evaluate the expression and store the result in the local variable
        ExpressionCodeGenerator
                .visitExpression(cw, mv, classNode, methodNode, assignmentStatementNode.expressionNode, varTable,subTable,debug);

        String expressionInstanceType= TypeResolver.getTypeExpressionNode(assignmentStatementNode.expressionNode,methodNode,subTable,varTable);

        mv.visitLabel(new Label());

        switch(expressionInstanceType){
            case "Int":
            case "Bool":
                mv.visitVarInsn(ISTORE, local_var_index);
                break;
            case "Float":
                mv.visitVarInsn(FSTORE, local_var_index);
                break;
            case "String":
            case "[Int]":
            case "[Float]":
                mv.visitIntInsn(ASTORE,local_var_index);
                break;
            default:
                throw new Exception("unconsidered case in AssignmentStatementCodeGenerator (visitAssignVarIntoVar). type was :"+expressionInstanceType);
        }
    }

    private static void visitAssignVarIntoArray(
            ClassWriter cw,
            ClassNode classNode,
            AssignmentStatementNode assignmentStatementNode,
            MethodVisitor mv,
            MethodNode methodNode,
            LocalVarSymbolTable varTable,
            SubroutineSymbolTable subTable,
            boolean debug
    )throws Exception{
        //arr=[1,2];
        //arr[0]=4;
        int local_var_index = varTable.getIndexOfVariable(assignmentStatementNode.variableNode.name);

        //push the array reference
        mv.visitIntInsn(ALOAD,local_var_index);

        //push the index
        ExpressionCodeGenerator
                .visitExpression(
                        cw,mv,classNode,methodNode,
                        assignmentStatementNode.variableNode.indexOptional.get(),
                        varTable,subTable,debug
                );


        //evaluate the expression and store the result in the local variable
        ExpressionCodeGenerator
                .visitExpression(cw, mv, classNode, methodNode, assignmentStatementNode.expressionNode, varTable,subTable,debug);

        String expressionInstanceType= TypeResolver.getTypeExpressionNode(assignmentStatementNode.expressionNode,methodNode,subTable,varTable);

        switch(expressionInstanceType){
            case "Int":
            case "Bool":
                mv.visitInsn(IASTORE);
                break;
            case "Float":
                mv.visitInsn(FASTORE);
                break;
            case "String":
            case "[Int]":
            case "[Float]":
                mv.visitInsn(ASTORE);
                break;
            default:
                throw new Exception("unconsidered case in AssignmentStatementCodeGenerator. type was :"+expressionInstanceType);
        }
    }
}
