package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import static org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator.unique;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.StatementDracoVMCodeGenerator.generateDracoVMCodeForStatement;

public class IfStatementDracoVMCodeGenerator {


  public static void genVMCodeForIfStatement(IfStatementNode ifstmt, MethodNode containerMethod, DracoVMCodeWriter sb, LocalVarSymbolTable varTable) throws Exception{

    long unique=unique();
    String startlabel = "ifstart"+unique;
    String elselabel = "else"+unique;
    String endlabel = "ifend"+unique;

    sb.label(startlabel);

    //push the expression
    genDracoVMCodeForExpression(ifstmt.condition,sb,varTable);
    sb.neg();
    //if condition is false, jump to else
    sb.if_goto(elselabel);

    for(StatementNode stmt : ifstmt.statements){
      generateDracoVMCodeForStatement(stmt,containerMethod,sb,varTable);
    }

    sb._goto(endlabel);
    sb.label(elselabel);

    for(StatementNode stmt : ifstmt.elseStatements){
      generateDracoVMCodeForStatement(stmt,containerMethod,sb,varTable);
    }

    sb.label(endlabel);
  }
}
