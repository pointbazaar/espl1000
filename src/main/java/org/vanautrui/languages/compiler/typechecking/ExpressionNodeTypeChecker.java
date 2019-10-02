package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.OperatorNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.Arrays;
import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.TermNodeTypeChecker.typecheckTermNode;

public class ExpressionNodeTypeChecker {

  static void typeCheckExpressionNode(
          List<AST> asts, ClassNode classNode, MethodNode methodNode,
          ExpressionNode expr, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable
  ) throws Exception{
    //int + int
    //int - int
    //int * int
    //int / int

    //expression is compiled to evaluate in order. meaning
    //that if there are 3 terms,
    //the first gets evaluated, then the second, then the operator gets applied
    //such for convienience we check for now that the terms have all the same type

    //as of now i think we should first focus on integer addition
    //and let the other cases throw an exception
    //they should be implemented later

    //the types should be all the same for now
    typecheckTermNode(asts,classNode,methodNode,expr.term,subTable,varTable);
    ITypeNode type= TypeResolver.getTypeTermNode(expr.term,methodNode,subTable,varTable);
    List<String> currentAllowedTypes= Arrays.asList("PInt","Float");

    if(expr.termNodes.size()==1 && expr.operatorNodes.size()==1 && expr.operatorNodes.get(0).operator.equals("==")) {
      //can only compare stuff which is the same
      ITypeNode otherType = TypeResolver.getTypeTermNode(expr.termNodes.get(0),methodNode,subTable,varTable);
      if(!otherType.getTypeName().equals(type.getTypeName())){
        throw new Exception(TypeChecker.class.getSimpleName()+": to compare with '==', they have to be the same type ");
      }
      typecheckTermNode(asts,classNode,methodNode,expr.termNodes.get(0),subTable,varTable);
      return;
    }

    if(!currentAllowedTypes.contains(type.getTypeName())){
      if(expr.termNodes.size()==0){
        //we typechecked the type before, as long as it is by itself, there is nothing to do
        return;
      }else{
        throw new Exception(type+" is not in the currently allowed types for use in expression with multiple terms");
      }
    }

    for (TermNode t : expr.termNodes){
      if( !( TypeResolver.getTypeTermNode(t,methodNode,subTable,varTable).equals(type) ) ){
        throw new Exception("for now, all types in an expression must be the same");
      }
      //typecheck the term node, maybe it contains identifiers that are not declared?
      typecheckTermNode(asts,classNode,methodNode,t,subTable,varTable);
    }

    List<String> currentAllowedOPs=Arrays.asList("+","-","*","/");
    for(OperatorNode op : expr.operatorNodes){
      if(!currentAllowedOPs.contains(op.operator)){
        throw new Exception("currently not supported operator: "+op.operator);
      }
    }
    //TODO: look for the other cases
  }
}
