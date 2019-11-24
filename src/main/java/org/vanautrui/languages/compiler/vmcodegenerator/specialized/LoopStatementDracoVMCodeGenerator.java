package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.ArrayList;
import java.util.List;

import static org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator.unique;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.StatementDracoVMCodeGenerator.generateDracoVMCodeForStatement;

public class LoopStatementDracoVMCodeGenerator {


  static List<String> genVMCodeForLoopStatement(
          LoopStatementNode loop,
          MethodNode containerMethod,
          SymbolTableContext ctx
  ) throws Exception {

    final List<String> vm = new ArrayList<>();

    final long unique=unique();
    final String startlabel = "loopstart"+unique;
    final String endlabel = "loopend"+unique;

    //push the expression
    vm.addAll(genDracoVMCodeForExpression(loop.count, ctx)); //+1

    vm.add("dup"); //+1

    vm.add("label "+startlabel);

    //if counter is 0, jump to end
    vm.add("iconst 0");
    vm.add("ieq");
    vm.add("if-goto "+endlabel);

    //execute statements
    for(StatementNode stmt : loop.statements){
      vm.addAll(generateDracoVMCodeForStatement(stmt,containerMethod,ctx));
    }

    //subtract 1 from the counter
    vm.add("dec");

    //duplicate top of stack so we can compare again
    vm.add("dup"); //+1
    vm.add("goto "+startlabel);

    vm.add("label "+endlabel);
    vm.add("pop");  //-1

    return vm;
  }
}
