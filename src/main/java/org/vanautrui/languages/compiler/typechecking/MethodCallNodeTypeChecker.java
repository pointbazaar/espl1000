package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SubroutineTypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.ExpressionNodeTypeChecker.typeCheckExpressionNode;

public final class MethodCallNodeTypeChecker {


  public synchronized static void typeCheckMethodCallNode(
    List<AST> asts,
    NamespaceNode namespaceNode,
    MethodNode methodNode,
    MethodCallNode methodCallNode,
    SubroutineSymbolTable subTable,
    LocalVarSymbolTable varTable,
    StructsSymbolTable structsTable
  ) throws Exception {
    boolean found = false;

    if (subTable.containsSubroutine(methodCallNode.methodName)) {
      found = true;
    }

    if (
            varTable.containsVariable(methodCallNode.methodName)
                    && varTable.get(methodCallNode.methodName).getType().type instanceof SubroutineTypeNode
    ) {
      found = true;
    }

    //search in arguments, for a subroutine argument
    if(methodNode.arguments.stream().filter(declaredArgumentNode -> {
        if(declaredArgumentNode.type.type instanceof BasicTypeWrappedNode){
          IBasicAndWrappedTypeNode t1=((BasicTypeWrappedNode) declaredArgumentNode.type.type).typenode;
          if(t1 instanceof SubroutineTypeNode){
            return true;
          }
        }
        return false;
      }).count()>0
    ){
      found=true;
    }

    if (!found) {
      System.out.println(subTable.toString());
      throw
              new Exception(
                      "name of method not in subroutine symbol table " +
                              "and not in local variable table (or not a subroutine variable): "
                              + methodCallNode.methodName
              );
    }

    typeCheckMethodCallArguments(asts,namespaceNode,methodNode,methodCallNode,subTable,varTable,structsTable);
    //for static method calls, check that the class exists
  }

  private static void typeCheckMethodCallArguments(
          List<AST> asts, NamespaceNode namespaceNode,
          MethodNode methodNode, MethodCallNode methodCallNode,
          SubroutineSymbolTable subTable, LocalVarSymbolTable varTable,
          StructsSymbolTable structsTable
  )throws Exception {

    final int nargs_expected = subTable.getNumberOfArgumentsOfSubroutine(methodCallNode.methodName);
    final int nargs_supplied = methodCallNode.argumentList.size();

    //number of arguments supplied should equal the number of arguments declared
    if(nargs_expected != nargs_supplied){
      throw new Exception(methodCallNode.methodName+" expected "+nargs_expected+" but only got "+nargs_supplied+" arguments");
    }

    //check that the types of the arguments match the types declared
    for(int i=0;i<nargs_expected;i++){

      final TypeNode arg_type = TypeResolver.getTypeExpressionNode(methodCallNode.argumentList.get(i), methodNode, subTable, varTable, structsTable);
      final TypeNode arg_type_expected = subTable.getArgTypeOfSubroutineAtIndex(methodCallNode.methodName,i);

      if(!arg_type.getTypeName().equals(arg_type_expected.getTypeName())){
        //check if maybe the expected type is a wider type that contains the supplied type
        if(TypeChecker.isIntegralType(arg_type) && arg_type_expected.getTypeName().equals("Integer")){
          //it is ok
        }else {

          throw new Exception("in call of " + methodCallNode.toSourceCode()
                  + " , the types at index " + i + " did not match. expected type was " + arg_type_expected.getTypeName() + ", but " + arg_type.getTypeName() + " was supplied");
        }
      }
    }

    //all arguments should typecheck
    for (ExpressionNode expr : methodCallNode.argumentList) {
      typeCheckExpressionNode(asts, namespaceNode, methodNode, expr, subTable, varTable,structsTable);
    }
  }
}
