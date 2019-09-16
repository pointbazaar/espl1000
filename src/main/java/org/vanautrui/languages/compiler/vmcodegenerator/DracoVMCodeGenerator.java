package org.vanautrui.languages.compiler.vmcodegenerator;

import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.OperatorNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.IStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.*;
import org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import java.util.*;

public class DracoVMCodeGenerator {

    public static List<String> generateDracoVMCode(Set<AST> asts, SubroutineSymbolTable subTable) throws Exception{

        List<String> sb =new ArrayList<>();
        //TODO:
        for(AST ast :asts){
            for(ClassNode classNode : ast.classNodeList){
                for(MethodNode methodNode : classNode.methodNodeList){

                    generateDracoVMCodeForMethod(methodNode,sb,subTable);
                }
                if(classNode.fieldNodeList.size()>0){
                    throw new Exception("unhandled case in dracovm code generation");
                }
            }
        }

        return sb;
    }

    private static void generateDracoVMCodeForMethod(MethodNode m, List<String> sb,SubroutineSymbolTable subTable)throws Exception{

        LocalVarSymbolTable varTable = SymbolTableGenerator.createMethodScopeSymbolTable(m,subTable);

        sb.add("subroutine "+m.methodName.methodName.name+" "+m.arguments.size());
        //not sure if it is number of arguments or number of local vars

        //TODO: setup a new stack frame
        //push ebp
        //mov esp ebp

        for(StatementNode stmt : m.statements){
            generateDracoVMCodeForStatement(stmt,m,sb,varTable);
        }

        //return should be the last statement in every possible branch for these statements

        //remove the arguments off the stack
        for(int i=0;i<m.arguments.size();i++){
            sb.add("pop");
        }
    }

    private static void generateDracoVMCodeForStatement(StatementNode stmt,MethodNode containerMethod,List<String> sb,LocalVarSymbolTable varTable)throws Exception{
        IStatementNode snode = stmt.statementNode;
        if(snode instanceof MethodCallNode){
            MethodCallNode call = (MethodCallNode)snode;
            genVMCodeForMethodCall(call,sb,varTable);
        }else if(snode instanceof LoopStatementNode) {
            LoopStatementNode loop = (LoopStatementNode) snode;
            genVMCodeForLoopStatement(loop,containerMethod,sb,varTable);
        }else if(snode instanceof AssignmentStatementNode) {
            AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode) snode;
            genVMCodeForAssignmentStatement(assignmentStatementNode,sb);
        }else if(snode instanceof WhileStatementNode){
            WhileStatementNode whileStatementNode =(WhileStatementNode)snode;
            genVMCodeForWhileStatement(whileStatementNode,containerMethod,sb,varTable);
        }else if(snode instanceof IfStatementNode) {
            IfStatementNode ifStatementNode = (IfStatementNode) snode;
            genVMCodeForIfStatement(ifStatementNode,containerMethod,sb,varTable);
        }else if(snode instanceof ReturnStatementNode){
            ReturnStatementNode returnStatementNode = (ReturnStatementNode)snode;
            genDracoVMCodeForReturn(returnStatementNode,containerMethod,sb,varTable);
        }else{
            throw new Exception("unconsidered statement type: "+stmt.statementNode.getClass().getName());
        }
    }

    private static void genVMCodeForAssignmentStatement(AssignmentStatementNode assignmentStatementNode, List<String> sb) throws Exception {
        //TODO
        throw new Exception("unhandled case");
    }

    private static void genVMCodeForIfStatement(IfStatementNode ifstmt,MethodNode containerMethod, List<String> sb,LocalVarSymbolTable varTable) throws Exception{

        long unique=unique();
        String startlabel = "ifstart"+unique;
        String elselabel = "else"+unique;
        String endlabel = "ifend"+unique;

        sb.add("label "+startlabel);

        //push the expression
        genDracoVMCodeForExpression(ifstmt.condition,sb,varTable);
        sb.add("neg");
        //if condition is false, jump to else
        sb.add("if-goto "+elselabel);

        for(StatementNode stmt : ifstmt.statements){
            generateDracoVMCodeForStatement(stmt,containerMethod,sb,varTable);
        }

        sb.add("goto "+endlabel);
        sb.add("label "+elselabel);

        for(StatementNode stmt : ifstmt.elseStatements){
            generateDracoVMCodeForStatement(stmt,containerMethod,sb,varTable);
        }

        sb.add("label "+endlabel);
    }

    private static void genVMCodeForWhileStatement(WhileStatementNode whileStmt,MethodNode containerMethod, List<String> sb,LocalVarSymbolTable varTable)throws Exception {

        long unique=unique();
        String startlabel = "whilestart"+unique;
        String endlabel = "whileend"+unique;

        sb.add("label "+startlabel);

        //push the expression
        genDracoVMCodeForExpression(whileStmt.condition,sb,varTable);
        sb.add("neg");
        //if condition is false, jump to end
        sb.add("if-goto "+endlabel);

        //execute statements
        for(StatementNode stmt : whileStmt.statements){
            generateDracoVMCodeForStatement(stmt,containerMethod,sb,varTable);
        }


        sb.add("goto "+startlabel);

        sb.add("label "+endlabel);
    }

    private static void genVMCodeForLoopStatement(LoopStatementNode loop,MethodNode containerMethod, List<String> sb,LocalVarSymbolTable varTable) throws Exception {

        long unique=unique();
        String startlabel = "loopstart"+unique;
        String endlabel = "loopend"+unique;

        //push the expression
        genDracoVMCodeForExpression(loop.count,sb,varTable);
        sb.add("dup");

        sb.add("label "+startlabel);

        //if counter is 0, jump to end
        sb.add("iconst 0");
        sb.add("eq");
        sb.add("if-goto "+endlabel);

        //execute statements
        for(StatementNode stmt : loop.statements){
            generateDracoVMCodeForStatement(stmt,containerMethod,sb,varTable);
        }

        //subtract 1 from the counter
        sb.add("iconst 1");
        sb.add("sub");

        //duplicate top of stack so we can compare again
        sb.add("dup");
        sb.add("goto "+startlabel);

        sb.add("label "+endlabel);

        sb.add("pop");
    }

    private static void genVMCodeForFloatConst(FloatConstNode fconst,List<String> sb){
        sb.add("fconst "+fconst.value);
    }

    private static void genVMCodeForIntConst(int iconst,List<String> sb){
        sb.add("iconst "+iconst);
    }

    private static void genVMCodeForBoolConst(BoolConstNode bconst,List<String> sb){
        sb.add("iconst "+((bconst.value)?1:0));
    }

    private static void genDracoVMCodeForTerm(TermNode tNode,List<String> sb,LocalVarSymbolTable varTable)throws Exception{
        ITermNode t = tNode.termNode;
        if(t instanceof FloatConstNode){
            genVMCodeForFloatConst((FloatConstNode)t,sb);
        }else if(t instanceof IntConstNode){
            genVMCodeForIntConst(((IntConstNode)t).value,sb);
        }else if(t instanceof ExpressionNode) {
            ExpressionNode expressionNode = (ExpressionNode)t;
            genDracoVMCodeForExpression(expressionNode,sb,varTable);
            throw new Exception("currently unhandled case ");
        }else if(t instanceof VariableNode) {
            //find the local variable index
            // and push the variable onto the stack
            VariableNode variableNode = (VariableNode) t;
            genDracoVMCodeForVariable(variableNode,sb,varTable);
        }else if(t instanceof MethodCallNode){
            MethodCallNode methodCallNode = (MethodCallNode)t;
            genVMCodeForMethodCall(methodCallNode,sb,varTable);
        }else if(t instanceof BoolConstNode) {
            genVMCodeForBoolConst((BoolConstNode)t,sb);
        }else if(t instanceof ArrayConstantNode) {
            ArrayConstantNode arrayConstantNode = (ArrayConstantNode) t;
            //TODO
            throw new Exception("currently unhandled case");
        }else if (t instanceof CharConstNode) {
            CharConstNode t1 = (CharConstNode) t;
            genVMCodeForIntConst((int)t1.content,sb);
        }else{
            throw new Exception("unhandled case");
        }
    }

    private static void genDracoVMCodeForVariable(VariableNode variableNode, List<String> sb, LocalVarSymbolTable varTable) throws Exception{
        //push the variable on the stack

        //if the variable is local,
        //or argument, that would be different segments
        //we have to consider its index

        String name = variableNode.name;

        int index=varTable.getIndexOfVariable(name);
        String segment = varTable.getSegment(name);

        sb.add("push "+segment+" "+index);
    }

    private static void genVMCodeForMethodCall(MethodCallNode methodCallNode, List<String> sb,LocalVarSymbolTable varTable) throws Exception {
        //push arguments on stack in reverse order
        for(int i=methodCallNode.argumentList.size()-1;i>=0;i--){
            ExpressionNode arg = methodCallNode.argumentList.get(i);
            genDracoVMCodeForExpression(arg,sb,varTable);
        }
        sb.add("call "+methodCallNode.identifierMethodName);
    }

    private static void genDracoVMCodeForOp(OperatorNode opNode,List<String> sb)throws Exception{
        switch (opNode.operator){
            case "+":
                sb.add("add");
                break;
            case "-":
                sb.add("sub");
                break;
            default:
                throw new Exception("currently unsupported op "+opNode.operator);
        }
    }

    private static void genDracoVMCodeForExpression(ExpressionNode expr,List<String> sb,LocalVarSymbolTable varTable)throws Exception{
        genDracoVMCodeForTerm(expr.term,sb,varTable);
        for(int i=0;i<expr.termNodes.size();i++){
            genDracoVMCodeForTerm(expr.termNodes.get(i),sb,varTable);
            genDracoVMCodeForOp(expr.operatorNodes.get(i),sb);
        }
    }

    private static void genDracoVMCodeForReturn(ReturnStatementNode retStmt,MethodNode containerMethod,List<String> sb,LocalVarSymbolTable varTable)throws Exception {
        genDracoVMCodeForExpression(retStmt.returnValue,sb,varTable);
        if(containerMethod.methodName.methodName.name.equals("main")){
            sb.add("exit");
        }else{
            sb.add("return");
        }
    }

    public static long unique(){
        //uniqueness for jump labels
        Random r = new Random();
        return Math.abs(r.nextInt(100000));
    }
}
