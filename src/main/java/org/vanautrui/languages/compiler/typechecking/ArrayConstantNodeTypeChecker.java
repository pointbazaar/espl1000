package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import static org.vanautrui.languages.compiler.typechecking.ExpressionNodeTypeChecker.typeCheckExpressionNode;

public final class ArrayConstantNodeTypeChecker {


  public synchronized static void typeCheckArrayConstantNode(
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final MethodNode methodNode,
          final ArrayConstantNode arrConstNode,
          final SymbolTableContext ctx
          ) throws Exception
  {

    final SubroutineSymbolTable subTable = ctx.subTable;
    final LocalVarSymbolTable varTable = ctx.varTable;
    final StructsSymbolTable structsTable = ctx.structsTable;

    //all the types of the elements should be the same
    if (arrConstNode.elements.size() > 0) {

      final TypeNode type_of_elements =
              TypeResolver.getTypeExpressionNode(arrConstNode.elements.get(0), ctx);

      for (final ExpressionNode expr : arrConstNode.elements) {
        final TypeNode element_type = TypeResolver.getTypeExpressionNode(expr, ctx);
        if (!element_type.getTypeName().equals(type_of_elements.getTypeName())) {
          throw new Exception("type of the array items was inferred to "
                  + type_of_elements
                  + " from the first element's type, but type differed at index "
                  + arrConstNode.elements.indexOf(expr) + " : it's type was " + element_type);
        }
        typeCheckExpressionNode(asts, namespaceNode, methodNode, expr, ctx);
      }
    }
  }

}
