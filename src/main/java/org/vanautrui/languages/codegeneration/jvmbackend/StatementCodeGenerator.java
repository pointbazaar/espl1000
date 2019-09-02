package org.vanautrui.languages.codegeneration.jvmbackend;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.vanautrui.languages.symboltables.tables.LocalVarSymbolTable;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.typeresolution.TypeResolver;

import java.util.Arrays;
import java.util.List;

import static org.objectweb.asm.Opcodes.*;
import static org.vanautrui.languages.codegeneration.jvmbackend.LoopStatementCodeGenerator.visitLoopStatmentNode;

public class StatementCodeGenerator {

    public static void visitStatement(
            ClassWriter cw,
            MethodVisitor mv,
            ClassNode classNode,
            MethodNode methodNode,
            StatementNode statementNode,
            SubroutineSymbolTable subTable,
            LocalVarSymbolTable varTable,
            boolean debug
    ) throws Exception {

        //TODO: consider other statement types and such
        //statementNode.methodCallNode.visit(mv,classNode,methodNode);
        if(statementNode.statementNode instanceof MethodCallNode){
            MethodCallNode call = (MethodCallNode)statementNode.statementNode;
            MethodCallCodeGenerator.visitMethodCallNode(cw,mv,classNode,methodNode,call,varTable,subTable,debug);

            //TODO: if the method was not Void, we should pop something off the stack
            String returnType = TypeResolver.getTypeMethodCallNode(call,subTable);

            List<String> java_void_methods = Arrays.asList("print","println");
            if(!java_void_methods.contains(call.identifierMethodName)){
                mv.visitInsn(POP);
            }
        }else if(statementNode.statementNode instanceof LoopStatementNode) {
            LoopStatementNode loop = (LoopStatementNode) statementNode.statementNode;
            visitLoopStatmentNode(cw, mv, classNode, methodNode, loop,varTable,subTable,debug);
        }else if(statementNode.statementNode instanceof AssignmentStatementNode) {
            AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode) statementNode.statementNode;
            AssignmentStatementCodeGenerator.visitAssignmentStatementNode(cw,classNode,assignmentStatementNode,mv,methodNode,varTable,subTable,debug);
        }else if(statementNode.statementNode instanceof WhileStatementNode){
            WhileStatementNode whileStatementNode =(WhileStatementNode)statementNode.statementNode;
            WhileStatementCodeGenerator.visitWhileStatmentNode(cw,mv,classNode,methodNode,whileStatementNode,varTable,subTable,debug);
        }else if(statementNode.statementNode instanceof IfStatementNode) {
            IfStatementNode ifStatementNode = (IfStatementNode) statementNode.statementNode;
            IfStatementCodeGenerator.visitIfStatmentNode(cw, mv, classNode, methodNode, ifStatementNode, varTable, subTable,debug);
        }else if(statementNode.statementNode instanceof ReturnStatementNode){
            ReturnStatementNode returnStatementNode = (ReturnStatementNode)statementNode.statementNode;
            ReturnStatementCodeGenerator.visitReturnStatement(cw,mv,classNode,methodNode,returnStatementNode,varTable,subTable,debug);
        }else{
            throw new Exception("unconsidered statement type: "+statementNode.statementNode.getClass().getName());
        }
    }
}
