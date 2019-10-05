package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.ExpressionNodeTypeChecker.typeCheckExpressionNode;

public final class ArrayConstantNodeTypeChecker {


  public synchronized static void typeCheckArrayConstantNode(List<AST> asts, NamespaceNode namespaceNode, MethodNode methodNode,
                                         ArrayConstantNode arrConstNode, SubroutineSymbolTable subTable,
                                         LocalVarSymbolTable varTable) throws Exception
  {
    //all the types of the elements should be the same
    if (arrConstNode.elements.size() > 0) {

      ITypeNode type_of_elements =
              TypeResolver.getTypeExpressionNode(arrConstNode.elements.get(0), methodNode, subTable, varTable);

      for (ExpressionNode expr : arrConstNode.elements) {
        ITypeNode element_type = TypeResolver.getTypeExpressionNode(expr, methodNode, subTable, varTable);
        if (!element_type.getTypeName().equals(type_of_elements.getTypeName())) {
          throw new Exception("type of the array items was inferred to "
                  + type_of_elements
                  + " from the first element's type, but type differed at index "
                  + arrConstNode.elements.indexOf(expr) + " : it's type was " + element_type);
        }
        typeCheckExpressionNode(asts, namespaceNode, methodNode, expr, subTable, varTable);
      }
    }
  }

}
