package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.VariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.*;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.MethodCallDracoVMCodeGenerator.genVMCodeForMethodCall;

public class TermDracoVMCodeGenerator {


  public static void genDracoVMCodeForTerm(
          TermNode tNode,
          DracoVMCodeWriter sb,
          SubroutineSymbolTable subTable,
          LocalVarSymbolTable varTable,
          StructsSymbolTable structsTable
  )throws Exception{
    ITermNode t = tNode.termNode;
    if(t instanceof FloatConstNode){
      DracoVMCodeGenerator.genVMCodeForFloatConst((FloatConstNode)t,sb);
    }else if(t instanceof IntConstNode){
      DracoVMCodeGenerator.genVMCodeForIntConst(((IntConstNode)t).value,sb);
    }else if(t instanceof ExpressionNode) {
      ExpressionNode expressionNode = (ExpressionNode)t;
      genDracoVMCodeForExpression(expressionNode,sb,subTable,varTable,structsTable);
    }else if(t instanceof VariableNode) {
      //find the local variable index
      // and push the variable onto the stack
      VariableNode variableNode = (VariableNode) t;

      VariableDracoVMCodeGenerator.genDracoVMCodeForVariable(variableNode,sb,subTable,varTable,structsTable);
    }else if(t instanceof MethodCallNode){
      MethodCallNode methodCallNode = (MethodCallNode)t;
      genVMCodeForMethodCall(methodCallNode,sb,subTable,varTable,structsTable);

    }else if(t instanceof BoolConstNode) {
      DracoVMCodeGenerator.genVMCodeForBoolConst((BoolConstNode)t,sb);
    }else if(t instanceof ArrayConstantNode) {
      ArrayConstantNode arrayConstantNode = (ArrayConstantNode) t;
      DracoVMCodeGenerator.genVMCodeForArrayConstant(arrayConstantNode,sb,subTable,varTable,structsTable);
    }else if (t instanceof CharConstNode) {
      final CharConstNode t1 = (CharConstNode) t;

      final int ascii_value = t1.content;

      DracoVMCodeGenerator.genVMCodeForIntConst(ascii_value,sb);
    }else{
      throw new Exception("unhandled case");
    }
  }
}
