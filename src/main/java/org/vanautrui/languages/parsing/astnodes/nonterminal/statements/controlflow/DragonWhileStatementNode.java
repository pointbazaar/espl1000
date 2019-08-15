package org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow;

import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.KeywordToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.IDragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIntegerConstantNode;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.Set;

public class DragonWhileStatementNode implements IDragonASTNode, IDragonStatementNode {

    public DragonExpressionNode condition;

    public List<DragonStatementNode> statements=new ArrayList<>();

    public DragonWhileStatementNode(DragonTokenList tokens)throws Exception{

        DragonTokenList copy = new DragonTokenList(tokens);

        copy.expectAndConsumeOtherWiseThrowException(new KeywordToken("while"));

        this.condition =new DragonExpressionNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("{"));

        //maybe there be some statements
        boolean success_statements = true;
        while (success_statements) {
            try {
                this.statements.add(new DragonStatementNode(copy));
            } catch (Exception e) {
                success_statements = false;
            }
        }

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("}"));

        tokens.set(copy);
    }


    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {
        //the condition expression should be of type boolean
        if(!this.condition.getType().equals("Bool") && !this.condition.getType().equals("Boolean")){
            throw new Exception(" condition should be of type boolean");
        }
        for(DragonStatementNode stmt : this.statements){
            stmt.doTypeCheck(asts,currentClass,currentMethod);
        }
    }

    @Override
    public String toSourceCode() {
        return null;
    }
}
