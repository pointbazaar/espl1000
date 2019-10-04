package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.DeclaredArgumentNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IdentifierNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.SubroutineTypeNode;
import org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.Arrays;
import java.util.List;
import java.util.Set;

import static org.vanautrui.languages.compiler.typechecking.ClassNodeTypeChecker.typeCheckClassNode;
import static org.vanautrui.languages.compiler.typechecking.ExpressionNodeTypeChecker.typeCheckExpressionNode;
import static org.vanautrui.languages.compiler.typechecking.ITypeNodeTypeChecker.typeCheckITypeNode;
import static org.vanautrui.languages.compiler.typechecking.StatementNodeTypeChecker.typeCheckStatementNode;

public class TypeChecker {

  //this class is supposed to typecheck the program.
  //it receives the 'environment' of a node as arguments
  //so each node can check if it is correct,
  //i.e. that it matches the type expected by its environment
  //and that itself contains only
  //AST Nodes that conform to the expected types.


  //the primitive types and their arrays
  public static final List<String> primitive_types_and_arrays_of_them =
          Arrays.asList(
                  "PInt", // Int which is  >= 0
                  "NInt", // Int which is <= 0
                  "Integer",
                  "Float", "Bool", "Char",
                  "[PInt]", "[NInt]",
                  "[Integer]",
                  "[Float]", "[Bool]", "[Char]"
          );

  public static void doTypeCheck(List<AST> asts, boolean debug) throws Exception {
    if (debug) {
      System.out.println("TYPECHECKING");
    }
    SubroutineSymbolTable subroutineSymbolTable = SymbolTableGenerator.createSubroutineSymbolTable(asts, debug);
    if (debug) {
      System.out.println("generate subroutine symbol table:");
      System.out.println(subroutineSymbolTable.toString());
    }
    for (AST ast : asts) {
      for (ClassNode classNode : ast.classNodeList) {
        typeCheckClassNode(asts, classNode, subroutineSymbolTable, debug);
      }
    }
  }



  static void typeCheckIfStatementNode(List<AST> asts, ClassNode classNode,
                                       MethodNode methodNode,
                                       IfStatementNode ifStatementNode,
                                       SubroutineSymbolTable subTable,
                                       LocalVarSymbolTable varTable) throws Exception {
    //the condition expression should be of type boolean
    ITypeNode conditionType =
            TypeResolver.getTypeExpressionNode(ifStatementNode.condition, methodNode, subTable, varTable);

    if (!conditionType.getTypeName().equals("Bool")) {
      throw new Exception(" condition should be of type Bool, but is of type: " + conditionType.getTypeName());
    }
    for (StatementNode stmt : ifStatementNode.statements) {
      typeCheckStatementNode(asts, classNode, methodNode, stmt, subTable, varTable);
    }
    for (StatementNode stmt : ifStatementNode.elseStatements) {
      typeCheckStatementNode(asts, classNode, methodNode, stmt, subTable, varTable);
    }
  }

  static void typeCheckMethodCallNode(List<AST> asts, ClassNode classNode,
                                      MethodNode methodNode, MethodCallNode methodCallNode,
                                      SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception {
    boolean found = false;

    if (subTable.containsSubroutine(methodCallNode.methodName)) {
      found = true;
    }

    if (
            varTable.containsVariable(methodCallNode.methodName)
            && varTable.get(methodCallNode.methodName).getType() instanceof SubroutineTypeNode
    ) {
      found = true;
    }

    if (!found) {
      System.out.println(subTable.toString());
      throw
          new Exception(
                  "name of method not in subroutine symbol table and not in local variable table (or not a subroutine variable): "
                  + methodCallNode.methodName
          );
    }

    //for static method calls, check that the class exists

    //all arguments should typecheck
    for (ExpressionNode expr : methodCallNode.argumentList) {
      typeCheckExpressionNode(asts, classNode, methodNode, expr, subTable, varTable);
    }
  }


  static void typeCheckArrayConstantNode(List<AST> asts, ClassNode classNode, MethodNode methodNode,
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
        typeCheckExpressionNode(asts, classNode, methodNode, expr, subTable, varTable);
      }
    }
  }


  static void typeCheckWhileStatementNode(
          List<AST> asts,
          ClassNode classNode,
          MethodNode methodNode,
          WhileStatementNode whileStatementNode,
          SubroutineSymbolTable subTable,
          LocalVarSymbolTable varTable
  ) throws Exception {
    //the condition expression should be of type boolean
    ITypeNode conditionType =
            TypeResolver.getTypeExpressionNode(whileStatementNode.condition, methodNode, subTable, varTable);

    if (!conditionType.getTypeName().equals("Bool")) {
      throw new Exception(" condition should be of type Bool : '"
              + whileStatementNode.condition.toSourceCode()
              + "' but was of type: " + conditionType);
    }
    for (StatementNode stmt : whileStatementNode.statements) {
      typeCheckStatementNode(asts, classNode, methodNode, stmt, subTable, varTable);
    }
  }

  static void typeCheckLoopStatementNode(
          List<AST> asts, ClassNode classNode, MethodNode methodNode,
          LoopStatementNode loopStatementNode, SubroutineSymbolTable subTable,
          LocalVarSymbolTable varTable
  ) throws Exception {
    //the condition expression should be of type boolean

    ITypeNode countType = TypeResolver.getTypeExpressionNode(loopStatementNode.count, methodNode, subTable, varTable);
    if (!countType.getTypeName().equals("PInt")) {
      throw
          new Exception(
                  " condition should be of an Integral Type >= 0 (PInt) . this is a loop statement after all."
          );
    }
    for (StatementNode stmt : loopStatementNode.statements) {
      typeCheckStatementNode(asts, classNode, methodNode, stmt, subTable, varTable);
    }
  }


  public static boolean isIntegralType(ITypeNode type) {
    return Arrays.asList("PInt", "NInt", "Integer").contains(type.getTypeName());
  }


  static void typeCheckMethodNameNode(List<AST> asts, ClassNode classNode, String methodName) throws Exception {
    //method names should not be duplicate in a class
    //this may change in another version of dragon

    long count = classNode.methodNodeList.stream().filter(mNode -> mNode.methodName.equals(methodName)).count();

    if (count > 1) {
      throw new Exception("duplicate declaration of method '" + methodName + "' ");
    }
  }

  static void typeCheckDeclaredArgumentNode(List<AST> asts,
                                            ClassNode classNode,
                                            DeclaredArgumentNode declaredArgumentNode) throws Exception {
    typeCheckITypeNode(asts, classNode, declaredArgumentNode.type.typenode);
  }

  private void typeCheckIdentifierNode(Set<AST> asts,
                                       ClassNode classNode,
                                       MethodNode methodNode,
                                       IdentifierNode identifierNode)
  throws Exception
  {
    //TODO: it should check that the identifier is
    //declared in method scope or class scope.
    //so there should be some declaration of it
    //TODO: also check that the identifier is not duplicate declaration

    //identifiers can only be used within a class or method so
    //there should be a context

    //it is not declared in class scope, it should be declared in
    //method scope

    for (DeclaredArgumentNode arg : methodNode.arguments) {
      if (arg.name.equals(identifierNode.name)) {
        return;
      }
    }

    //TODO: search if identifier is declared as a variable
    //by a statement above its usage in the method
    //this should maybe be done in another implementation
    //of this method in another node class,
    //perhaps in MethodNode.doTypeCheck();

    throw new Exception("could not find declaration for usage of Identifier '" + identifierNode.name + "'");
  }
}
