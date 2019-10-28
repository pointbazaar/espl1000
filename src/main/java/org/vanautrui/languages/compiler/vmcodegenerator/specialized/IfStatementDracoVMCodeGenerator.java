package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import static org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator.unique;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.StatementDracoVMCodeGenerator.generateDracoVMCodeForStatement;

public final class IfStatementDracoVMCodeGenerator {


  public static void genVMCodeForIfStatement(
          IfStatementNode ifstmt, MethodNode containerMethod,
          DracoVMCodeWriter sb,
          SubroutineSymbolTable subTable,
          LocalVarSymbolTable varTable,
          StructsSymbolTable structsTable
  ) throws Exception{

    long unique=unique();
    String startlabel = "ifstart"+unique;
    String elselabel = "else"+unique;
    String endlabel = "ifend"+unique;

    sb.label(startlabel);

    //push the expression
    genDracoVMCodeForExpression(ifstmt.condition,sb,subTable,varTable,structsTable);
    sb.not();
    //if condition is false, jump to else
    sb.if_goto(elselabel);

    for(StatementNode stmt : ifstmt.statements){
      generateDracoVMCodeForStatement(stmt,containerMethod,sb,subTable,varTable,structsTable);
    }

    sb._goto(endlabel);
    sb.label(elselabel);

    for(StatementNode stmt : ifstmt.elseStatements){
      generateDracoVMCodeForStatement(stmt,containerMethod,sb,subTable,varTable,structsTable);
    }

    sb.label(endlabel);
  }
}
