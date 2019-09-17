package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;

public class MethodCallDracoVMCodeGenerator {


  public static void genVMCodeForMethodCall(MethodCallNode methodCallNode, DracoVMCodeWriter sb, LocalVarSymbolTable varTable) throws Exception {
    //push arguments on stack in reverse order
    for(int i=methodCallNode.argumentList.size()-1;i>=0;i--){
      ExpressionNode arg = methodCallNode.argumentList.get(i);
      genDracoVMCodeForExpression(arg,sb,varTable);
    }
    sb.call(methodCallNode.methodName);
  }
}
