package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.ArrayList;
import java.util.List;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.StatementDracoVMCodeGenerator.generateDracoVMCodeForStatement;

public final class SubroutineDracoVMCodeGenerator {


  public static List<String> generateDracoVMCodeForMethod(
          final NamespaceNode containerClass,
          final MethodNode m,
          final SubroutineSymbolTable subTable,
          final StructsSymbolTable structsTable,
          final boolean debug,
          final boolean printsymboltables
  )throws Exception{

    final List<String> vminstrs = new ArrayList<>();

    final LocalVarSymbolTable varTable = SymbolTableGenerator.createMethodScopeSymbolTable(m,subTable,structsTable);
    if(debug || printsymboltables){
      System.out.println(varTable.toString());
    }

    vminstrs.add("subroutine "+containerClass.name+"_"+m.methodName+" "+m.arguments.size()+" args "+subTable.getNumberOfLocalVariablesOfSubroutine(m.methodName)+" locals");


    //push the number of local variables on the stack
    for(int i=0;i<subTable.getNumberOfLocalVariablesOfSubroutine(m.methodName);i++){
      vminstrs.add("iconst 0"); //push local vars on the stack"
    }

    final SymbolTableContext ctx = new SymbolTableContext(subTable,varTable,structsTable);

    for(final StatementNode stmt : m.statements){
      vminstrs.addAll(generateDracoVMCodeForStatement(stmt,m,ctx));
    }

    //return should be the last statement in every possible branch for these statements
    return vminstrs;
  }
}
