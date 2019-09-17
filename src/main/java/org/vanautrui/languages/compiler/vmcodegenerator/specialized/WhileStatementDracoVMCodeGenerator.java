package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import static org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator.unique;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.StatementDracoVMCodeGenerator.generateDracoVMCodeForStatement;

public class WhileStatementDracoVMCodeGenerator {


  public static void genVMCodeForWhileStatement(WhileStatementNode whileStmt, MethodNode containerMethod, DracoVMCodeWriter sb, LocalVarSymbolTable varTable)throws Exception {

    long unique=unique();
    String startlabel = "whilestart"+unique;
    String endlabel = "whileend"+unique;

    sb.label(startlabel);

    //push the expression
    genDracoVMCodeForExpression(whileStmt.condition,sb,varTable);
    sb.neg();
    //if condition is false, jump to end
    sb.if_goto(endlabel);

    //execute statements
    for(StatementNode stmt : whileStmt.statements){
      generateDracoVMCodeForStatement(stmt,containerMethod,sb,varTable);
    }

    sb._goto(startlabel);
    sb.label(endlabel);
  }
}
