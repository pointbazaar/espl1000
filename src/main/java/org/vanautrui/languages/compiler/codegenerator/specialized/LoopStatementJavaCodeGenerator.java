package org.vanautrui.languages.compiler.codegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static org.vanautrui.languages.compiler.codegenerator.JavaCodeGenerator.unique;
import static org.vanautrui.languages.compiler.codegenerator.specialized.StatementJavaCodeGenerator.generateJavaCodeForStatement;

public final class LoopStatementJavaCodeGenerator {

  static List<String> genJavaCodeForLoopStatement(
          final LoopStatementNode loop,
          final MethodNode containerMethod,
          final SymbolTableContext ctx
  ) throws Exception {

    final List<String> vm = new ArrayList<>();

    final long unique=unique();
    final String counterName = "counter"+unique;

    //init our counter
    vm.add("final int "+counterName+"=");
    vm.add(ExpressionJavaCodeGenerator.genJavaCodeForExpression(loop.count,ctx));
    vm.add(";");

    vm.add("while ("+counterName+" > 0) {");

    //execute statements
    for(final StatementNode stmt : loop.statements){
      vm.addAll(
              generateJavaCodeForStatement(stmt,containerMethod,ctx)
                      .stream()
                      .map(s -> "\t"+s) //indent correctly
                      .collect(Collectors.toList())
      );
    }

    vm.add(counterName+"--");
    vm.add("}");

    return vm;
  }
}
