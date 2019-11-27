package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.VariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.List;

public final class VariableNodeTypeChecker {


  static void typeCheckVariableNode(
          List<AST> asts,
          NamespaceNode namespaceNode,
          MethodNode methodNode,
          VariableNode varNode,
          SubroutineSymbolTable subTable,
          LocalVarSymbolTable varTable,
          StructsSymbolTable structsTable
  ) throws Exception{
    //it should check that the variable is
    //declared in method scope or class scope.
    //so there should be some declaration of it

    if(varNode.simpleVariableNode.indexOptional.isPresent()){
      //if there is an index, it should be positive. we can check one of the bounds for free
      //by only accepting PInt type
      TypeNode index_type = TypeResolver.getTypeExpressionNode(varNode.simpleVariableNode.indexOptional.get(), subTable, varTable,structsTable);
      if(!index_type.getTypeName().equals("PInt")){
        throw new Exception("can only index into arrays with PInt type. Because an array index is >= 0.");
      }
    }


    //identifiers can only be used within a class or method so
    //there should be a context

    //search if identifier is declared as a local variable or argument variable
    if(varTable.containsVariable(varNode.simpleVariableNode.name)){
      return;
    }

    //search if identifier is declared as a subroutine
    if(subTable.containsSubroutine(varNode.simpleVariableNode.name)){
      if(!varNode.simpleVariableNode.indexOptional.isPresent()){
        return;
      }else{
        throw new Exception("TypeChecker: '"+varNode.toSourceCode()+"' has been used with an index, : '"+varNode.toSourceCode()+"' , but it is a subroutine. you cannot index into subroutines");
      }
    }

    throw new Exception("could not find declaration for usage of variable '"+varNode.toSourceCode()+"' \n"+subTable.toString());
  }
}
