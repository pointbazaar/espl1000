package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.StructMemberDeclNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.StructDeclNode;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;

public final class StructMemberDeclTypeChecker {

    //prevents instance creation
    private StructMemberDeclTypeChecker(){}

    public static void typeCheckStructMemberDeclNode(
            final AST_Whole_Program asts,
            final NamespaceNode namespaceNode,
            final StructDeclNode structDeclNode,
            final StructMemberDeclNode structMemberDeclNode,
            final SubroutineSymbolTable subTable
    )throws Exception{

      //check that the member is only declared once
      long count=0;
      for(StructMemberDeclNode member : structDeclNode.members){
        if(member.name.equals(structMemberDeclNode.name)){count++;}
      }
      if(count!=1){
        throw new Exception("struct member "+structMemberDeclNode.name+" was declared "+count+" times. should have only been declared once.");
      }

      IBasicAndWrappedTypeNodeTypeChecker.typeCheckIBasicAndWrappedTypeNode(asts,namespaceNode,structMemberDeclNode.type.typeNode);
    }
}
