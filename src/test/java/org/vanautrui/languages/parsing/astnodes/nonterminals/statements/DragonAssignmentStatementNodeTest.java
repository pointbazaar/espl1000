package org.vanautrui.languages.parsing.astnodes.nonterminals.statements;

import org.junit.Test;
import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.lexing.tokens.IntegerConstantToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonAssignmentStatementNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonVariableNode;

public class DragonAssignmentStatementNodeTest {

    @Test
    public void test1()throws Exception{

        DragonTokenList tokens = new DragonTokenList();

        tokens.add(new IdentifierToken("x"));
        tokens.add(new SymbolToken("="));
        tokens.add(new IntegerConstantToken(4));
        tokens.add(new SymbolToken(";"));

        DragonAssignmentStatementNode assignmentStatementNode = new DragonAssignmentStatementNode(tokens);
    }
}
