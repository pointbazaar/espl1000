package org.vanautrui.languages.compiler.codegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static org.vanautrui.languages.compiler.codegenerator.specialized.ExpressionJavaCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.codegenerator.specialized.StatementJavaCodeGenerator.generateJavaCodeForStatement;

public final class IfStatementJavaCodeGenerator {

  public static List<String> genVMCodeForIfStatement(
          final IfStatementNode ifstmt,
          final MethodNode containerMethod,
          final SymbolTableContext ctx
  ) throws Exception{

    final List<String> vm = new ArrayList<>();

    vm.add("if (" + genDracoVMCodeForExpression(ifstmt.condition, ctx) + ") {");

    for(final StatementNode stmt : ifstmt.statements){
      vm.addAll(
              generateJavaCodeForStatement(stmt,containerMethod, ctx)
                      .stream()
                      .map(s -> "\t"+s) //indent correctly
                      .collect(Collectors.toList())
      );
    }

    if(!ifstmt.elseStatements.isEmpty()) {
      vm.add("} else {");
      for(final StatementNode stmt : ifstmt.elseStatements){
        vm.addAll(
                generateJavaCodeForStatement(stmt,containerMethod, ctx)
                        .stream()
                        .map(s -> "\t"+s) //indent correctly
                        .collect(Collectors.toList())
        );
      }
    }

    vm.add("}");
    return vm;
  }
}
