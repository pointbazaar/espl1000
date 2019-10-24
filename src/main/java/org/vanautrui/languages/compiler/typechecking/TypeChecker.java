package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.DeclaredArgumentNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IdentifierNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;
import org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;

import java.util.Arrays;
import java.util.List;
import java.util.Set;

import static org.vanautrui.languages.compiler.typechecking.NamespaceNodeTypeChecker.typeCheckNamespaceNode;

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

  public static void doTypeCheck(
          List<AST> asts,
          boolean debug
  ) throws Exception {
    if (debug) {
      System.out.println("TYPECHECKING");
    }
    final SubroutineSymbolTable subroutineSymbolTable = SymbolTableGenerator.createSubroutineSymbolTable(asts, debug);
    final StructsSymbolTable structsTable = SymbolTableGenerator.createStructsSymbolTable(asts, debug);
    if (debug) {
      System.out.println("generate subroutine symbol table:");
      System.out.println(subroutineSymbolTable.toString());
    }
    for (AST ast : asts) {
      for (NamespaceNode namespaceNode : ast.namespaceNodeList) {
        typeCheckNamespaceNode(asts, namespaceNode, subroutineSymbolTable, debug,structsTable);
      }
    }
    find_exactly_one_entry_point(asts);
  }

  private static void find_exactly_one_entry_point(List<AST> asts)throws Exception{
    int count=0;
    for (AST ast : asts) {
      for (NamespaceNode namespaceNode : ast.namespaceNodeList) {
        for(MethodNode methodNode : namespaceNode.methodNodeList){
          if(methodNode.methodName.equals("main")){
            count++;
          }
        }
      }
    }
    if(count!=1){
      throw new Exception("found more or less than 1 entry point '()~>PInt main'");
    }
  }






  public static boolean isIntegralType(TypeNode type) {
    return Arrays.asList("PInt", "NInt", "Integer").contains(type.getTypeName());
  }


  static void typeCheckMethodNameNode(List<AST> asts, NamespaceNode namespaceNode, String methodName) throws Exception {
    //method names should not be duplicate in a class
    //this may change in another version of dragon

    long count = namespaceNode.methodNodeList.stream().filter(mNode -> mNode.methodName.equals(methodName)).count();

    if (count > 1) {
      throw new Exception("duplicate declaration of method '" + methodName + "' ");
    }
  }



  private void typeCheckIdentifierNode(Set<AST> asts,
                                       NamespaceNode namespaceNode,
                                       MethodNode methodNode,
                                       IdentifierNode identifierNode)
  throws Exception
  {
    //it should check that the identifier is
    //declared in method scope or class scope.
    //so there should be some declaration of it
    //also check that the identifier is not duplicate declaration

    //identifiers can only be used within a class or method so
    //there should be a context

    //it is not declared in class scope, it should be declared in
    //method scope

    for (DeclaredArgumentNode arg : methodNode.arguments) {
      if (arg.name.equals(identifierNode.name)) {
        return;
      }
    }

    //search if identifier is declared as a variable
    //by a statement above its usage in the method
    //this should maybe be done in another implementation
    //of this method in another node class,
    //perhaps in MethodNode.doTypeCheck();

    throw new Exception("could not find declaration for usage of Identifier '" + identifierNode.name + "'");
  }
}
