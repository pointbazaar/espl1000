package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.StructMemberDeclNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.StructDeclNode;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;

import java.util.List;

public final class StructMemberDeclTypeChecker {

    public static void typeCheckStructMemberDeclNode(
            List<AST> asts,
            NamespaceNode namespaceNode,
            StructDeclNode structDeclNode,
            StructMemberDeclNode structMemberDeclNode,
            SubroutineSymbolTable subTable
    )throws Exception{

      //check that the member is only declared once
      long count=0;
      for(StructMemberDeclNode member : structDeclNode.structMembersList){
        if(member.name.equals(structMemberDeclNode.name)){count++;}
      }
      if(count!=1){
        throw new Exception("struct member "+structMemberDeclNode.name+" was declared "+count+" times. should have only been declared once.");
      }

      IBasicAndWrappedTypeNodeTypeChecker.typeCheckIBasicAndWrappedTypeNode(asts,namespaceNode,structMemberDeclNode.type.typenode);
    }
}
