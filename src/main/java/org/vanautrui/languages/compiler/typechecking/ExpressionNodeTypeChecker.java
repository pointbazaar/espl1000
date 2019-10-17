package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.OperatorNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.Arrays;
import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.TermNodeTypeChecker.typecheckTermNode;

public class ExpressionNodeTypeChecker {

  static void typeCheckExpressionNode(
          List<AST> asts, NamespaceNode namespaceNode, MethodNode methodNode,
          ExpressionNode expr, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable
  ) throws Exception{
    //expression is compiled to evaluate in order. meaning
    //that if there are 3 terms,
    //the first gets evaluated, then the second, then the operator gets applied
    //such for convienience we check for now that the terms have all the same type

    //check expression with only 1 operand
    if(expr.operatorNodes.size()==0 && expr.termNodes.size()==0){
      typecheckTermNode(asts, namespaceNode,methodNode,expr.term,subTable,varTable);
    }else if(expr.operatorNodes.size()==1 && expr.termNodes.size()==1){
      //check expressions with only 2 operands
      typeCheckExpressionNodeWith2Operands(asts, namespaceNode,methodNode,expr.term,expr.operatorNodes.get(0),expr.termNodes.get(0),subTable,varTable);
    }else{
      typeCheckExpressionNodeWithMoreThan2Operands(asts, namespaceNode,methodNode,expr,subTable,varTable);
    }
  }

  private static void typeCheckExpressionNodeWithMoreThan2Operands(List<AST> asts, NamespaceNode namespaceNode, MethodNode methodNode, ExpressionNode expr, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception {

    //the types should be all the same for now
    typecheckTermNode(asts, namespaceNode,methodNode,expr.term,subTable,varTable);
    TypeNode type= TypeResolver.getTypeTermNode(expr.term,methodNode,subTable,varTable);
    final List<String> currentAllowedTypes= Arrays.asList("PInt","Float");
    final List<String> allowed_operators_for_expressions_with_more_than_2_terms=Arrays.asList("+","-");


    if(expr.termNodes.size()==1 && expr.operatorNodes.size()==1 && expr.operatorNodes.get(0).operator.equals("==")) {
      //can only compare stuff which is the same
      TypeNode otherType = TypeResolver.getTypeTermNode(expr.termNodes.get(0),methodNode,subTable,varTable);
      if(!otherType.getTypeName().equals(type.getTypeName())){
        throw new Exception(TypeChecker.class.getSimpleName()+": to compare with '==', they have to be the same type ");
      }
      typecheckTermNode(asts, namespaceNode,methodNode,expr.termNodes.get(0),subTable,varTable);
      return;
    }

    if(!currentAllowedTypes.contains(type.getTypeName())){
        throw new Exception(type+" is not in the currently allowed types for use in expression with more than 2 terms");
    }

    for (TermNode t : expr.termNodes){
      if( !( TypeResolver.getTypeTermNode(t,methodNode,subTable,varTable).equals(type) ) ){
        throw new Exception("for now, all types in an expression must be the same");
      }
      //typecheck the term node, maybe it contains identifiers that are not declared?
      typecheckTermNode(asts, namespaceNode,methodNode,t,subTable,varTable);
    }

    for(OperatorNode op : expr.operatorNodes){
      if(!allowed_operators_for_expressions_with_more_than_2_terms.contains(op.operator)){
        throw new Exception("currently not supported operator: "+op.operator);
      }
    }
  }

  private static void typeCheckExpressionNodeWith2Operands(List<AST> asts, NamespaceNode namespaceNode, MethodNode methodNode, TermNode term1, OperatorNode opNode, TermNode term2, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception {



    final List<String> arithmetic_operators_for_epressions_with_2_operands=Arrays.asList("+","-","*","/");
    final List<String> bitshifting_operators_for_epressions_with_2_operands=Arrays.asList("<<",">>");

    if(bitshifting_operators_for_epressions_with_2_operands.contains(opNode.operator)){
      typeCheckExpressionNodeWith2OperandsBitShifting(asts, namespaceNode,methodNode,term1,opNode,term2,subTable,varTable);
    }else if(arithmetic_operators_for_epressions_with_2_operands.contains(opNode.operator)){
      final List<String> currentAllowedTypes= Arrays.asList("PInt","Float");

      final TypeNode type1 = TypeResolver.getTypeTermNode(term1,methodNode,subTable,varTable);
      final TypeNode type2 = TypeResolver.getTypeTermNode(term2,methodNode,subTable,varTable);

      if(!currentAllowedTypes.contains(type1.getTypeName())){
        throw new Exception(""+type1.getTypeName()+" is not a supported type for expressions with 2 terms.");
      }

      if(!currentAllowedTypes.contains(type2.getTypeName())){
        throw new Exception(""+type2.getTypeName()+" is not a supported type for expressions with 2 terms.");
      }


      if(!arithmetic_operators_for_epressions_with_2_operands.contains(opNode.operator)){
        throw new Exception("operator '"+opNode.operator+"' not supported for expression with 2 Terms.");
      }

      if(!type1.getTypeName().equals(type2.getTypeName())){
        throw new Exception("in an expression with 2 terms, both have to have the same type. But "+type1.getTypeName()+" and "+type2.getTypeName()+" are different types.");
      }

      typecheckTermNode(asts, namespaceNode,methodNode,term1,subTable,varTable);
      typecheckTermNode(asts, namespaceNode,methodNode,term2,subTable,varTable);
    }
  }

  private static void typeCheckExpressionNodeWith2OperandsBitShifting(List<AST> asts, NamespaceNode namespaceNode, MethodNode methodNode, TermNode term1, OperatorNode opNode, TermNode term2, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception {
    final List<String> bitshifting_operators_for_epressions_with_2_operands=Arrays.asList("<<",">>");

    final String current_allowed_type_for_bitshifting="PInt";

    final TypeNode type1 = TypeResolver.getTypeTermNode(term1,methodNode,subTable,varTable);
    final TypeNode type2 = TypeResolver.getTypeTermNode(term2,methodNode,subTable,varTable);

    if(!type1.getTypeName().equals(current_allowed_type_for_bitshifting) || !type2.getTypeName().equals(current_allowed_type_for_bitshifting)){
      throw new Exception("one or two of the operands for bitshifting were not of type "+current_allowed_type_for_bitshifting);
    }

    typecheckTermNode(asts, namespaceNode,methodNode,term1,subTable,varTable);
    typecheckTermNode(asts, namespaceNode,methodNode,term2,subTable,varTable);
  }
}
