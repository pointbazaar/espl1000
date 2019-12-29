package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SubroutineTypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import static org.vanautrui.languages.compiler.typechecking.ExpressionNodeTypeChecker.typeCheckExpressionNode;

public final class MethodCallNodeTypeChecker {

  //prevents instance creation
  private MethodCallNodeTypeChecker(){}

  public synchronized static void typeCheckMethodCallNode(
    final AST_Whole_Program asts,
    final NamespaceNode namespaceNode,
    final MethodNode methodNode,
    final MethodCallNode methodCallNode,
    final SymbolTableContext ctx
  ) throws Exception {

    final SubroutineSymbolTable subTable = ctx.subTable;
    final LocalVarSymbolTable varTable = ctx.varTable;

    boolean found = false;

    found = subTable.containsSubroutine(methodCallNode.methodName)
            || varTable.containsVariable(methodCallNode.methodName);

    //search in arguments, for a subroutine argument
    if(methodNode.arguments.stream().anyMatch(declaredArgumentNode -> {
      if (declaredArgumentNode.type.typeNode instanceof BasicTypeWrappedNode) {
        final IBasicAndWrappedTypeNode t1 = ((BasicTypeWrappedNode) declaredArgumentNode.type.typeNode).typeNode;
        return t1 instanceof SubroutineTypeNode;
      }
      return false;
    })
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

    typeCheckMethodCallArguments(asts,namespaceNode,methodNode,methodCallNode,ctx);
    //for static method calls, check that the class exists
  }

  private static void typeCheckMethodCallArguments(
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final MethodNode methodNode,
          final MethodCallNode methodCallNode,
          final SymbolTableContext ctx
          )throws Exception {

    final SubroutineSymbolTable subTable = ctx.subTable;

    final int nargs_expected = subTable.getNumberOfArgumentsOfSubroutine(methodCallNode.methodName);
    final int nargs_supplied = methodCallNode.arguments.size();

    //number of arguments supplied should equal the number of arguments declared
    if(nargs_expected != nargs_supplied){
      throw new Exception(methodCallNode.methodName+" expected "+nargs_expected+" but only got "+nargs_supplied+" arguments");
    }

    //check that the types of the arguments match the types declared
    for(int i=0;i<nargs_expected;i++){

      final TypeNode arg_type = TypeResolver.getTypeExpressionNode(methodCallNode.arguments.get(i), ctx);
      final TypeNode arg_type_expected = subTable.getArgTypeOfSubroutineAtIndex(methodCallNode.methodName,i);

      if(!arg_type.getTypeName().equals(arg_type_expected.getTypeName())){
        //check if maybe the expected type is a wider type that contains the supplied type
        if(TypeChecker.isIntegralType(arg_type) && arg_type_expected.getTypeName().equals("Int")) {
          //it is ok
        }else  if(arg_type_expected.getTypeName().equals("#")){
          //it is ok, as # means anything
        }else {

          String msg = "in call of " + methodCallNode.toSourceCode()
                  + " , the types at index " + i + " did not match. expected type was '"
                  + arg_type_expected.getTypeName() + "' , but '" + arg_type.getTypeName() + "' was supplied";

          msg+=" in context "+methodNode.toSourceCode();

          throw new Exception(msg);
        }
      }
    }

    //all arguments should typecheck
    for (final ExpressionNode expr : methodCallNode.arguments) {
      typeCheckExpressionNode(asts, namespaceNode, methodNode, expr, ctx);
    }
  }
}
