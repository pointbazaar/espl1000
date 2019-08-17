package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.lexing.tokens.utils.DragonToken;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.IDragonTermNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonDeclaredArgumentNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonAssignmentStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassFieldNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

import java.util.Optional;
import java.util.Set;

public class DragonVariableNode implements IDragonASTNode , IDragonTermNode {

    //this identifies a variable

    public IdentifierToken name;

    public DragonVariableNode(DragonTokenList tokens) throws Exception {

        DragonToken token = tokens.get(0);

        if (token instanceof IdentifierToken) {
            this.name = ((IdentifierToken) token);
            tokens.consume(1);

        } else {
            throw new Exception("could not read variable name");
        }

    }

    @Override
    public String toSourceCode() {
        return this.name.getContents();
    }


    @Override
    public String getType(DragonMethodNode methodNode) throws Exception {
        //TODO: implement by looking at the definitions in the AST and such

        //go through previous statements and look for the assignment which was first made to a primitive value
        //
        String type="";
        for(DragonStatementNode stmt : methodNode.statements){
            if(stmt.statementNode instanceof DragonAssignmentStatementNode){
                DragonAssignmentStatementNode assignmentStatementNode = (DragonAssignmentStatementNode)stmt.statementNode;

                //TODO: what if we assign something to itself?
                //that would probably cause an endless loop
                //TODO: FIX IT
                type = assignmentStatementNode.expressionNode.getType(methodNode);
                break;
            }
        }
        if(type.equals("")){
            throw new Exception("could not determine type of "+this.name.getContents());
        }

        return type;
    }
}
