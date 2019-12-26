package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.OperatorNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.Arrays;
import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.TermNodeTypeChecker.typecheckTermNode;

public final class ExpressionNodeTypeChecker {

  private static final List<String> currentAllowedTypes = Arrays.asList("PInt","NInt","Int","Float");

  private static final List<String> integral_types  = Arrays.asList("PInt","NInt","Int");

  static void typeCheckExpressionNode(
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final MethodNode methodNode,
          final ExpressionNode expr,
          final SymbolTableContext ctx
  ) throws Exception{
    //expression is compiled to evaluate in order. meaning
    //that if there are 3 terms,
    //the first gets evaluated, then the second, then the operator gets applied
    //such for convienience we check for now that the terms have all the same type

    final SubroutineSymbolTable subTable = ctx.subTable;
    final LocalVarSymbolTable varTable = ctx.varTable;
    final StructsSymbolTable structsTable = ctx.structsTable;

    //check expression with only 1 operand
    if(expr.operatorNodes.size()==0 && expr.termNodes.size()==0){
      typecheckTermNode(asts, namespaceNode,methodNode,expr.term,ctx);
    }else if(expr.operatorNodes.size()==1 && expr.termNodes.size()==1){
      //check expressions with only 2 operands
      typeCheckExpressionNodeWith2Operands(asts, namespaceNode,methodNode,expr.term,expr.operatorNodes.get(0),expr.termNodes.get(0),ctx);
    }else{
      typeCheckExpressionNodeWithMoreThan2Operands(asts, namespaceNode,methodNode,expr,ctx);
    }
  }

  private static void typeCheckExpressionNodeWithMoreThan2Operands(
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final MethodNode methodNode,
          final ExpressionNode expr,
          final SymbolTableContext ctx
  ) throws Exception {

    //the types should be all the same for now
    typecheckTermNode(asts, namespaceNode,methodNode,expr.term,ctx);
    TypeNode type= TypeResolver.getTypeTermNode(expr.term,ctx);
    final List<String> allowed_operators_for_expressions_with_more_than_2_terms=Arrays.asList("+","-");


    if(expr.termNodes.size()==1 && expr.operatorNodes.size()==1 && expr.operatorNodes.get(0).operator.equals("==")) {
      //can only compare stuff which is the same
      TypeNode otherType = TypeResolver.getTypeTermNode(expr.termNodes.get(0),ctx);
      if(!otherType.getTypeName().equals(type.getTypeName())){
        throw new Exception(TypeChecker.class.getSimpleName()+": to compare with '==', they have to be the same type ");
      }
      typecheckTermNode(asts, namespaceNode,methodNode,expr.termNodes.get(0),ctx);
      return;
    }

    if(!currentAllowedTypes.contains(type.getTypeName())){
        throw new Exception(type+" is not in the currently allowed types for use in expression with more than 2 terms");
    }

    for (TermNode t : expr.termNodes){
      if( !( TypeResolver.getTypeTermNode(t,ctx).equals(type) ) ){
        throw new Exception("for now, all types in an expression must be the same");
      }
      //typecheck the term node, maybe it contains identifiers that are not declared?
      typecheckTermNode(asts, namespaceNode,methodNode,t,ctx);
    }

    for(OperatorNode op : expr.operatorNodes){
      if(!allowed_operators_for_expressions_with_more_than_2_terms.contains(op.operator)){
        throw new Exception("currently not supported operator: "+op.operator);
      }
    }
  }

  private static void typeCheckExpressionNodeWith2Operands(
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final MethodNode methodNode,
          final TermNode term1,
          final OperatorNode opNode,
          final TermNode term2,
          final SymbolTableContext ctx
    ) throws Exception {

    final SubroutineSymbolTable subTable = ctx.subTable;
    final LocalVarSymbolTable varTable = ctx.varTable;
    final StructsSymbolTable structsTable = ctx.structsTable;

    final List<String> arithmetic_operators_for_epressions_with_2_operands=Arrays.asList("+","-","*","/");
    final List<String> bitshifting_operators_for_epressions_with_2_operands=Arrays.asList("<<",">>");

    if(bitshifting_operators_for_epressions_with_2_operands.contains(opNode.operator)){
      typeCheckExpressionNodeWith2OperandsBitShifting(asts, namespaceNode,methodNode,term1,opNode,term2,ctx);
    }else if(arithmetic_operators_for_epressions_with_2_operands.contains(opNode.operator)){

      final TypeNode type1 = TypeResolver.getTypeTermNode(term1,ctx);
      final TypeNode type2 = TypeResolver.getTypeTermNode(term2,ctx);

      if(!currentAllowedTypes.contains(type1.getTypeName())){
        throw new Exception(""+type1.getTypeName()+" is not a supported type for expressions with 2 terms.");
      }

      if(!currentAllowedTypes.contains(type2.getTypeName())){
        throw new Exception(""+type2.getTypeName()+" is not a supported type for expressions with 2 terms. In Context TODO");
      }

      if(!arithmetic_operators_for_epressions_with_2_operands.contains(opNode.operator)){
        throw new Exception("operator '"+opNode.operator+"' not supported for expression with 2 Terms.");
      }

      if(!type1.getTypeName().equals(type2.getTypeName())){

        if(!
            (
                integral_types.contains(type1.getTypeName())
                && integral_types.contains(type2.getTypeName())
            )
        ) {

          throw new Exception("in an expression with 2 terms, both have to have the same type. But " + type1.getTypeName() + " and " + type2.getTypeName() + " are different types.");
        }
      }

      typecheckTermNode(asts, namespaceNode,methodNode,term1,ctx);
      typecheckTermNode(asts, namespaceNode,methodNode,term2,ctx);
    }
  }

  private static void typeCheckExpressionNodeWith2OperandsBitShifting(
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final MethodNode methodNode,
          final TermNode term1,
          final OperatorNode opNode,
          final TermNode term2,
          final SymbolTableContext ctx
  ) throws Exception {

    final String current_allowed_type_for_bitshifting="PInt";

    final TypeNode type1 = TypeResolver.getTypeTermNode(term1,ctx);
    final TypeNode type2 = TypeResolver.getTypeTermNode(term2,ctx);

    if(!type1.getTypeName().equals(current_allowed_type_for_bitshifting) || !type2.getTypeName().equals(current_allowed_type_for_bitshifting)){
      throw new Exception("one or two of the operands for bitshifting were not of type "+current_allowed_type_for_bitshifting);
    }

    typecheckTermNode(asts, namespaceNode,methodNode,term1,ctx);
    typecheckTermNode(asts, namespaceNode,methodNode,term2,ctx);
  }
}
