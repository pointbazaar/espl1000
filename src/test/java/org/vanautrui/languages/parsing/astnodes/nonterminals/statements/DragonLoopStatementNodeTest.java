package org.vanautrui.languages.parsing.astnodes.nonterminals.statements;

import org.junit.Test;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.lexing.tokens.IntegerConstantToken;
import org.vanautrui.languages.lexing.tokens.KeywordToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonLoopStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonMethodCallNode;

public class DragonLoopStatementNodeTest {

    @Test
    public void test1() throws Exception {
        DragonTokenList list = new DragonTokenList();
        list.add(new KeywordToken("loop"));
        list.add(new IntegerConstantToken(5));
        list.add(new SymbolToken("{"));

        list.add(new SymbolToken("}"));

        DragonLoopStatementNode loop = new DragonLoopStatementNode(list);
    }
}
