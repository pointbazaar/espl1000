package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.DeclaredArgumentNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IdentifierNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.symboltablegenerator.SymbolTableGenerator;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import static org.vanautrui.languages.compiler.typechecking.NamespaceNodeTypeChecker.typeCheckNamespaceNode;

public final class TypeChecker {

  //prevents instance creation
  private TypeChecker(){}

  //this class is supposed to typecheck the program.
  //it receives the 'environment' of a node as arguments
  //so each node can check if it is correct,
  //i.e. that it matches the type expected by its environment
  //and that itself contains only
  //AST Nodes that conform to the expected types.

  private static final List<String> primitive_types = Arrays.asList(
          "PInt",
          "NInt",
          "Int",
          "Float",
          "Bool",
          "Char"
  );

  private static final List<String> primitive_types_arrays = primitive_types.stream().map(t-> ("["+t+"]")).collect(Collectors.toList());

  //the primitive types and their arrays
  public static final List<String> primitive_types_and_arrays_of_them =
          Stream.concat(primitive_types.stream(),primitive_types_arrays.stream()).collect(Collectors.toList());


  public static void doTypeCheck(
          AST_Whole_Program asts,
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

    for (NamespaceNode namespaceNode : asts.namespaceNodeList) {
      typeCheckNamespaceNode(asts, namespaceNode, subroutineSymbolTable, debug,structsTable);
    }

    find_exactly_one_entry_point(asts);
  }

  private static void find_exactly_one_entry_point(final AST_Whole_Program asts)throws Exception{
    int count=0;
    for (NamespaceNode namespaceNode : asts.namespaceNodeList) {
      for(MethodNode methodNode : namespaceNode.methodNodeList){
        if(methodNode.methodName.equals("main")){
          count++;
        }
      }
    }

    final String hint = "'fn main ()~>PInt '";

    if(count==0){
      throw new Exception("found no entry point "+hint);
    }else if(count>1){
      throw new Exception("found more than 1 entry point "+hint);
    }
  }

  public static boolean isIntegralType(final TypeNode type) {
    return Arrays.asList("PInt", "NInt", "Integer","Int").contains(type.getTypeName());
  }


  static void typeCheckMethodNameNode(
          final AST_Whole_Program ast,
          final NamespaceNode namespaceNode,
          final String methodName
  ) throws Exception {
    //method names should not be duplicate in a class
    //this may change in another version of dragon

    long count = namespaceNode.methodNodeList.stream().filter(mNode -> mNode.methodName.equals(methodName)).count();

    if (count > 1) {
      throw new Exception("duplicate declaration of method '" + methodName + "' ");
    }
  }

  private void typeCheckIdentifierNode(
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final MethodNode methodNode,
          final IdentifierNode identifierNode
  ) throws Exception {
    //it should check that the identifier is
    //declared in method scope.
    //so there should be some declaration of it
    //also check that the identifier is not duplicate declaration

    //identifiers can only be used within a class or method so
    //there should be a context

    //it is not declared in class scope, it should be declared in
    //method scope

    for (final DeclaredArgumentNode arg : methodNode.arguments) {
      if (arg.name.get().equals(identifierNode.name)) {
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
