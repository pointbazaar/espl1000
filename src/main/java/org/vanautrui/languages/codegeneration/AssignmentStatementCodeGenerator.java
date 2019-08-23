package org.vanautrui.languages.codegeneration;

import org.objectweb.asm.ClassWriter;
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
        int local_var_index = varTable.getIndexOfVariable(assignmentStatementNode.variableNode.name);

        //evaluate the expression and store the result in the local variable
        ExpressionCodeGenerator
                .visitExpression(cw, mv, classNode, methodNode, assignmentStatementNode.expressionNode, varTable,subTable,debug);

        String expressionInstanceType= TypeResolver.getTypeExpressionNode(assignmentStatementNode.expressionNode,methodNode,subTable,varTable);

        //TODO: consider the type of the value we are assigning to
        //if we are storing into an array we need other bytecodes

        switch(expressionInstanceType){
            case "Int":
            case "Bool":
                if(assignmentStatementNode.variableNode.indexOptional.isPresent()){
                    mv.visitIntInsn(IASTORE,local_var_index);
                }else {
                    mv.visitVarInsn(ISTORE, local_var_index);
                }
                break;
            case "Float":
                mv.visitVarInsn(FSTORE, local_var_index);
                break;
            case "String":
                mv.visitVarInsn(ASTORE,local_var_index);
                break;
            case "[Int]":
            case "[Float]":
                mv.visitVarInsn(ASTORE,local_var_index);
                break;
            default:
                throw new Exception("unconsidered case in DragonStatementCodeGenerator. type was :"+expressionInstanceType);
        }
    }

    private void visitAssignVarIntoVar(
            ClassWriter cw,
            ClassNode classNode,
            AssignmentStatementNode assignmentStatementNode,
            MethodVisitor mv,
            MethodNode methodNode,
            LocalVarSymbolTable varTable,
            SubroutineSymbolTable subTable,
            boolean debug
    ){
        //x=3;y=4;
        //x=y;

        //both values are primitive

    }

    private void visitAssignReferenceIntoReference(){
        //arr1=arr2
        aoeu
    }

    private void visitAssignVarIntoArray(){
        //arr=[1,2];
        //arr[0]=4;
        aoeu
    }
}
