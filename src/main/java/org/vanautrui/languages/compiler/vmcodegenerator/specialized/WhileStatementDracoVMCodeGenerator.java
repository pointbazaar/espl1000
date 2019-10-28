package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeWriter;

import static org.vanautrui.languages.compiler.vmcodegenerator.DracoVMCodeGenerator.unique;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.StatementDracoVMCodeGenerator.generateDracoVMCodeForStatement;

public final class WhileStatementDracoVMCodeGenerator {


  public static void genVMCodeForWhileStatement(
          WhileStatementNode whileStmt,
          MethodNode containerMethod,
          DracoVMCodeWriter sb,
          SubroutineSymbolTable subTable,
          LocalVarSymbolTable varTable,
          StructsSymbolTable structsTable
  )throws Exception {

    long unique=unique();
    String startlabel = "whilestart"+unique;
    String endlabel = "whileend"+unique;

    sb.label(startlabel);

    //push the expression
    genDracoVMCodeForExpression(whileStmt.condition,sb,subTable,varTable,structsTable); //+1

    sb.not();
    //if condition is false, jump to end
    sb.if_goto(endlabel); //-1

    //execute statements
    for(StatementNode stmt : whileStmt.statements){
      generateDracoVMCodeForStatement(stmt,containerMethod,sb,subTable,varTable,structsTable);
    }

    sb._goto(startlabel);
    sb.label(endlabel);

  }
}
