package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.ArrayList;
import java.util.List;

import static org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator.unique;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.StatementDracoVMCodeGenerator.generateDracoVMCodeForStatement;

public final class IfStatementDracoVMCodeGenerator {


  public static List<String> genVMCodeForIfStatement(
          IfStatementNode ifstmt,
          MethodNode containerMethod,
          SymbolTableContext ctx
  ) throws Exception{


    final SubroutineSymbolTable subTable=ctx.subTable;
    final LocalVarSymbolTable varTable=ctx.varTable;
    final StructsSymbolTable structsTable=ctx.structsTable;

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

    for(StatementNode stmt : ifstmt.statements){
      vm.addAll(generateDracoVMCodeForStatement(stmt,containerMethod, ctx));
    }

    vm.add("goto "+endlabel);
    vm.add("label "+elselabel);

    for(StatementNode stmt : ifstmt.elseStatements){
      vm.addAll(generateDracoVMCodeForStatement(stmt,containerMethod, ctx));
    }

    vm.add("label "+endlabel);

    return vm;
  }
}
