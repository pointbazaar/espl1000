package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.ArrayList;
import java.util.List;

import static org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator.unique;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.StatementDracoVMCodeGenerator.generateDracoVMCodeForStatement;

public final class IfStatementDracoVMCodeGenerator {


  public static List<String> genVMCodeForIfStatement(
          final IfStatementNode ifstmt,
          final MethodNode containerMethod,
          final SymbolTableContext ctx
  ) throws Exception{

    final List<String> vm = new ArrayList<>();

    final long unique=unique();
    final String startlabel = "ifstart"+unique;
    final String elselabel = "else"+unique;
    final String endlabel = "ifend"+unique;

    vm.add("label "+startlabel);

    //push the expression
    vm.addAll(genDracoVMCodeForExpression(ifstmt.condition, ctx));
    vm.add("not");
    //if condition is false, jump to else
    vm.add("if-goto "+elselabel);

    for(final StatementNode stmt : ifstmt.statements){
      vm.addAll(generateDracoVMCodeForStatement(stmt,containerMethod, ctx));
    }

    vm.add("goto "+endlabel);
    vm.add("label "+elselabel);

    for(final StatementNode stmt : ifstmt.elseStatements){
      vm.addAll(generateDracoVMCodeForStatement(stmt,containerMethod, ctx));
    }

    vm.add("label "+endlabel);

    return vm;
  }
}
