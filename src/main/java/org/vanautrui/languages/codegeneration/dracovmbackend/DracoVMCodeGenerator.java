package org.vanautrui.languages.codegeneration.dracovmbackend;

import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.IStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.symboltables.tables.SubroutineSymbolTable;

import java.util.Set;

public class DracoVMCodeGenerator {

    public static String generateDracoVMCode(Set<AST> asts, SubroutineSymbolTable subTable) throws Exception{

        StringBuilder sb=new StringBuilder("");
        //TODO:
        for(AST ast :asts){
            for(ClassNode classNode : ast.classNodeList){
                for(MethodNode methodNode : classNode.methodNodeList){
                    generateDracoVMCodeForMethod(methodNode,sb);
                }
                if(classNode.fieldNodeList.size()>0){
                    throw new Exception("unhandled case in dracovm code generation");
                }
            }
        }

        return sb.toString();
    }

    private static void generateDracoVMCodeForMethod(MethodNode m, StringBuilder sb)throws Exception{
        //TODO
        sb.append("subroutine ").append(m).append(" ").append(m.arguments.size()).append("\n");
        //not sure if it is number of arguments or number of local vars
        for(StatementNode stmt : m.statements){
            generateDracoVMCodeForStatement(stmt,sb);
        }
    }

    private static void generateDracoVMCodeForStatement(StatementNode stmt,StringBuilder sb)throws Exception{
        IStatementNode snode = stmt.statementNode;
        if(snode instanceof MethodCallNode){
            MethodCallNode call = (MethodCallNode)snode;
            //TODO
        }else if(snode instanceof LoopStatementNode) {
            LoopStatementNode loop = (LoopStatementNode) snode;
            //TODO
        }else if(snode instanceof AssignmentStatementNode) {
            AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode) snode;
            //TODO
        }else if(snode instanceof WhileStatementNode){
            WhileStatementNode whileStatementNode =(WhileStatementNode)snode;
            //TODO
        }else if(snode instanceof IfStatementNode) {
            IfStatementNode ifStatementNode = (IfStatementNode) snode;
            //TODO
        }else if(snode instanceof ReturnStatementNode){
            ReturnStatementNode returnStatementNode = (ReturnStatementNode)snode;
            genDracoVMCodeForReturn(returnStatementNode,sb);
        }else{
            throw new Exception("unconsidered statement type: "+stmt.statementNode.getClass().getName());
        }
    }

    private static void genDracoVMCodeForReturn(ReturnStatementNode retStmt,StringBuilder sb){
        //TODO
    }
}
