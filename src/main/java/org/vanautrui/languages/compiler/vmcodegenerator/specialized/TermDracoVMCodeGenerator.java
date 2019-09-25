package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.*;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import static org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator.*;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.MethodCallDracoVMCodeGenerator.genVMCodeForMethodCall;

public class TermDracoVMCodeGenerator {


  public static void genDracoVMCodeForTerm(TermNode tNode, DracoVMCodeWriter sb, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable)throws Exception{
    ITermNode t = tNode.termNode;
    if(t instanceof FloatConstNode){
      genVMCodeForFloatConst((FloatConstNode)t,sb);
    }else if(t instanceof IntConstNode){
      genVMCodeForIntConst(((IntConstNode)t).value,sb);
    }else if(t instanceof ExpressionNode) {
      ExpressionNode expressionNode = (ExpressionNode)t;
      genDracoVMCodeForExpression(expressionNode,sb,subTable,varTable);
    }else if(t instanceof VariableNode) {
      //find the local variable index
      // and push the variable onto the stack
      VariableNode variableNode = (VariableNode) t;
      genDracoVMCodeForVariable(variableNode,sb,subTable,varTable);
    }else if(t instanceof MethodCallNode){
      MethodCallNode methodCallNode = (MethodCallNode)t;
      genVMCodeForMethodCall(methodCallNode,sb,subTable,varTable);

    }else if(t instanceof BoolConstNode) {
      genVMCodeForBoolConst((BoolConstNode)t,sb);
    }else if(t instanceof ArrayConstantNode) {
      ArrayConstantNode arrayConstantNode = (ArrayConstantNode) t;
      genVMCodeForArrayConstant(arrayConstantNode,sb,subTable,varTable);
    }else if (t instanceof CharConstNode) {
      CharConstNode t1 = (CharConstNode) t;
      genVMCodeForIntConst((int)t1.content,sb);
    }else{
      throw new Exception("unhandled case");
    }
  }
}
