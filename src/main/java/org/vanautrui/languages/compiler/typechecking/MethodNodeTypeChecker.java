package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.DeclaredArgumentNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import static org.vanautrui.languages.compiler.typechecking.DeclaredArgumentNodeTypeChecker.typeCheckDeclaredArgumentNode;
import static org.vanautrui.languages.compiler.typechecking.ITypeNodeTypeChecker.typeCheckITypeNode;
import static org.vanautrui.languages.compiler.typechecking.StatementNodeTypeChecker.typeCheckStatementNode;
import static org.vanautrui.languages.compiler.typechecking.TypeChecker.typeCheckMethodNameNode;

public final class MethodNodeTypeChecker {

  static void typeCheckMethodNode(
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final MethodNode methodNode,
          final SubroutineSymbolTable subTable,
          final StructsSymbolTable structsTable
  ) throws Exception{

    //create the variable Symbol table, to typecheck the statements
    final LocalVarSymbolTable varTable = SymbolTableGenerator.createMethodScopeSymbolTable(methodNode,subTable,structsTable);

    final SymbolTableContext ctx = new SymbolTableContext(subTable,varTable,structsTable);

    typeCheckMethodNameNode(asts, namespaceNode,methodNode.methodName);

    typeCheckITypeNode(asts, namespaceNode,methodNode.returnType.type);
    for(final StatementNode stmt : methodNode.statements){
      typeCheckStatementNode(asts, namespaceNode,methodNode,stmt,ctx);
    }
    for(final DeclaredArgumentNode arg : methodNode.arguments){
      typeCheckDeclaredArgumentNode(asts, namespaceNode,arg);
    }

    //at the end of every method/function, there has to be some return statement
    //atleast for now

    final StatementNode statementNode = methodNode.statements.get(methodNode.statements.size() - 1);
    if(! (statementNode.statementNode instanceof ReturnStatementNode)){

      throw new Exception("error in typechecking : "+methodNode.methodName+" does not have a return statement as the last statement ");
    }
  }
}
