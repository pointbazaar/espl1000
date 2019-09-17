package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.IStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import static org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator.genVMCodeForAssignmentStatement;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.IfStatementDracoVMCodeGenerator.genVMCodeForIfStatement;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.LoopStatementDracoVMCodeGenerator.genVMCodeForLoopStatement;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.MethodCallDracoVMCodeGenerator.genVMCodeForMethodCall;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ReturnDracoVMCodeGenerator.genDracoVMCodeForReturn;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.WhileStatementDracoVMCodeGenerator.genVMCodeForWhileStatement;

public class StatementDracoVMCodeGenerator {


  public static void generateDracoVMCodeForStatement(StatementNode stmt, MethodNode containerMethod, DracoVMCodeWriter sb, LocalVarSymbolTable varTable)throws Exception{
    IStatementNode snode = stmt.statementNode;
    if(snode instanceof MethodCallNode){
      MethodCallNode call = (MethodCallNode)snode;
      genVMCodeForMethodCall(call,sb,varTable);
      //there is no assignment, and the return value is not used in an expression,
      // so the return value should be pop'd of the stack
      sb.pop();
    }else if(snode instanceof LoopStatementNode) {
      LoopStatementNode loop = (LoopStatementNode) snode;
      genVMCodeForLoopStatement(loop,containerMethod,sb,varTable);
    }else if(snode instanceof AssignmentStatementNode) {
      AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode) snode;
      genVMCodeForAssignmentStatement(assignmentStatementNode,sb,varTable);
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
}
