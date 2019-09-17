package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import static org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator.genDracoVMCodeForOp;
import static org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator.genDracoVMCodeForTerm;

public class ExpressionDracoVMCodeGenerator {

  public static void genDracoVMCodeForExpression(ExpressionNode expr, DracoVMCodeWriter sb, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable)throws Exception{
    genDracoVMCodeForTerm(expr.term,sb,subTable,varTable);
    for(int i=0;i<expr.termNodes.size();i++){
      genDracoVMCodeForTerm(expr.termNodes.get(i),sb,subTable,varTable);
      genDracoVMCodeForOp(expr.operatorNodes.get(i),sb);
    }
  }
}
