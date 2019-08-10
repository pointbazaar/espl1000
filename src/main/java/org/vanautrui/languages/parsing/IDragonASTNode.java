package org.vanautrui.languages.parsing;

import org.vanautrui.languages.ISourceCode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonMethodNode;

import java.util.Optional;
import java.util.Set;

public interface IDragonASTNode extends ISourceCode {


    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod)throws Exception;
    //this method is supposed to typecheck the program.
    //it receives the 'environment' of a node as arguments
    //so each node can check if it is correct,
    //i.e. that it matches the type expected by its environment
    //and that itself contains only
    //AST Nodes that conform to the expected types.
    //This method should be called recursively on all
    //children of the AST
}
