package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;

public class MethodCallDracoVMCodeGenerator {


  public static void genVMCodeForMethodCall(MethodCallNode methodCallNode, DracoVMCodeWriter sb, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception {
    //push arguments on stack in reverse order
    for(int i=methodCallNode.argumentList.size()-1;i>=0;i--){
      ExpressionNode arg = methodCallNode.argumentList.get(i);
      genDracoVMCodeForExpression(arg,sb,subTable,varTable);
    }
    sb.call(methodCallNode.methodName);

    int nArgs = subTable.getNumberOfArgumentsOfSubroutine(methodCallNode.methodName);
    //caller removes the arguments off the stack
    for(int i=0;i<nArgs;i++){

      //we must swap first, as there is the return value on top of stack
      sb.swap("remove previously pushed arguments off the stack");
      sb.pop("remove previously pushed arguments off the stack");
    }
  }
}
