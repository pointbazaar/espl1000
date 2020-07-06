package org.vanautrui.languages.compiler.codegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;
import org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

import static org.vanautrui.languages.compiler.codegenerator.specialized.StatementJavaCodeGenerator.generateJavaCodeForStatement;

public final class SubroutineJavaCodeGenerator {

  public static List<String> generateJavaCodeForMethod(
          final NamespaceNode containerClass,
          final MethodNode m,
          final SubroutineSymbolTable subTable,
          final StructsSymbolTable structsTable,
          final boolean debug,
          final boolean printsymboltables
  )throws Exception{

    final List<String> vm = new ArrayList<>();

    final LocalVarSymbolTable varTable = SymbolTableGenerator.createMethodScopeSymbolTable(m,subTable,structsTable);
    if(debug || printsymboltables){
      System.out.println(varTable.toString());
    }

    final String argsString = m.arguments.stream().map(arg -> arg.type+" "+arg.name.orElse("error")).collect(Collectors.joining(","));

    String typeNameInJava = m.returnType.getTypeName();
    if(Arrays.asList("PInt","NInt","NZInt","Int").contains(typeNameInJava.trim())){
      typeNameInJava = "int";
    }
    vm.add("\tpublic static "+typeNameInJava+" "+m.methodName+"("+argsString+") {");

    final SymbolTableContext ctx = new SymbolTableContext(subTable,varTable,structsTable);

    for(final StatementNode stmt : m.statements){
      vm.addAll(
              generateJavaCodeForStatement(stmt,m,ctx)
                      .stream()
                      .map(s -> "\t"+s) //indent correctly
                      .collect(Collectors.toList())
      );
    }

    vm.add("\t}");

    return vm;
  }
}
