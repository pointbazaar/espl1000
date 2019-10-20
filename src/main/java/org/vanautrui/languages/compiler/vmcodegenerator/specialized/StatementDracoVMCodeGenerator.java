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
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.IfStatementDracoVMCodeGenerator.genVMCodeForIfStatement;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.LoopStatementDracoVMCodeGenerator.genVMCodeForLoopStatement;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.MethodCallDracoVMCodeGenerator.genVMCodeForMethodCall;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ReturnDracoVMCodeGenerator.genDracoVMCodeForReturn;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.WhileStatementDracoVMCodeGenerator.genVMCodeForWhileStatement;

public class StatementDracoVMCodeGenerator {


  public static void generateDracoVMCodeForStatement(
          StatementNode stmt,
          MethodNode containerMethod,
          DracoVMCodeWriter sb,
          SubroutineSymbolTable subTable,
          LocalVarSymbolTable varTable,
          StructsSymbolTable structsTable
  )throws Exception{
    IStatementNode snode = stmt.statementNode;
    if(snode instanceof MethodCallNode){
      MethodCallNode call = (MethodCallNode)snode;
      genVMCodeForMethodCall(call,sb,subTable,varTable,structsTable);
      //there is no assignment, and the return value is not used in an expression,
      // so the return value should be pop'd of the stack
      sb.pop();
    }else if(snode instanceof LoopStatementNode) {
      LoopStatementNode loop = (LoopStatementNode) snode;
      genVMCodeForLoopStatement(loop,containerMethod,sb,subTable,varTable,structsTable);
    }else if(snode instanceof AssignmentStatementNode) {
      AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode) snode;
      AssignmentStatementDracoVMCodeGenerator.genVMCodeForAssignmentStatement(assignmentStatementNode,sb,subTable,varTable,structsTable);
    }else if(snode instanceof WhileStatementNode){
      WhileStatementNode whileStatementNode =(WhileStatementNode)snode;
      genVMCodeForWhileStatement(whileStatementNode,containerMethod,sb,subTable,varTable,structsTable);
    }else if(snode instanceof IfStatementNode) {
      IfStatementNode ifStatementNode = (IfStatementNode) snode;
      genVMCodeForIfStatement(ifStatementNode,containerMethod,sb,subTable,varTable,structsTable);
    }else if(snode instanceof ReturnStatementNode){
      ReturnStatementNode returnStatementNode = (ReturnStatementNode)snode;
      genDracoVMCodeForReturn(returnStatementNode,containerMethod,sb,subTable,varTable,structsTable);
    }else{
      throw new Exception("unconsidered statement type: "+stmt.statementNode.getClass().getName());
    }
  }
}
