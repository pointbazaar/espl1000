package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;

public class ReturnDracoVMCodeGenerator {
  public static void genDracoVMCodeForReturn(ReturnStatementNode retStmt, MethodNode containerMethod, DracoVMCodeWriter sb, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable)throws Exception {
    genDracoVMCodeForExpression(retStmt.returnValue,sb,subTable,varTable);
    if(containerMethod.methodName.equals("main")){
      sb.exit();
    }else{
      //there is the return value on the stack,
      // we must swap that with the return address of the calling function, in order to return
      sb.swap("swap return value with return address of the calling function in order to return");
      sb._return();
    }
  }
}
