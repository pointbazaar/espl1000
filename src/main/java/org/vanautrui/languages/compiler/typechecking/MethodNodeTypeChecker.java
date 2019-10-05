package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.DeclaredArgumentNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.DeclaredArgumentNodeTypeChecker.typeCheckDeclaredArgumentNode;
import static org.vanautrui.languages.compiler.typechecking.ITypeNodeTypeChecker.typeCheckITypeNode;
import static org.vanautrui.languages.compiler.typechecking.StatementNodeTypeChecker.typeCheckStatementNode;
import static org.vanautrui.languages.compiler.typechecking.TypeChecker.*;

public class MethodNodeTypeChecker {

  static void typeCheckMethodNode(List<AST> asts, NamespaceNode namespaceNode, MethodNode methodNode, SubroutineSymbolTable subTable) throws Exception{

    //create the variable Symbol table, to typecheck the statements
    LocalVarSymbolTable varTable = SymbolTableGenerator.createMethodScopeSymbolTable(methodNode,subTable);

    typeCheckMethodNameNode(asts, namespaceNode,methodNode.methodName);

    typeCheckITypeNode(asts, namespaceNode,methodNode.returnType.typenode);
    for(StatementNode stmt : methodNode.statements){
      typeCheckStatementNode(asts, namespaceNode,methodNode,stmt,subTable,varTable);
    }
    for(DeclaredArgumentNode arg : methodNode.arguments){
      typeCheckDeclaredArgumentNode(asts, namespaceNode,arg);
    }

    //at the end of every method/function, there has to be some return statement
    //atleast for now

    StatementNode statementNode = methodNode.statements.get(methodNode.statements.size() - 1);
    if(! (statementNode.statementNode instanceof ReturnStatementNode)){

      throw new Exception("error in typechecking : "+methodNode.methodName+" does not have a return statement as the last statement ");
    }
  }
}
