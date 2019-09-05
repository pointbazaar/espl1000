package org.vanautrui.languages.dragoncompiler.parsing.astnodes.nonterminals.statements;

import org.junit.Test;
import org.vanautrui.languages.dragoncompiler.lexing.utils.TokenList;
import org.vanautrui.languages.dragoncompiler.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.dragoncompiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.dragoncompiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.dragoncompiler.parsing.astnodes.nonterminal.statements.MethodCallNode;

public class MethodCallNodeTest {

    @Test
    public void test1() throws Exception {
        TokenList list = new TokenList();
        list.add(new IdentifierToken("main"));
        list.add(new SymbolToken("("));
        list.add(new SymbolToken(")"));
        list.add(new SymbolToken(";"));

        MethodCallNode call = new MethodCallNode(list);
    }

    @Test
    public void test2() throws Exception {
        TokenList list = new TokenList();
        list.add(new IdentifierToken("main"));
        list.add(new SymbolToken("("));
        list.add(new IntegerNonNegativeConstantToken(4));
        list.add(new SymbolToken(")"));
        list.add(new SymbolToken(";"));

        MethodCallNode call = new MethodCallNode(list);
    }

    @Test
    public void test3() throws Exception {
        TokenList list = new TokenList();
        list.add(new IdentifierToken("main"));
        list.add(new SymbolToken("("));
        list.add(new IdentifierToken("x"));
        list.add(new SymbolToken(")"));
        list.add(new SymbolToken(";"));

        MethodCallNode call = new MethodCallNode(list);
    }
}
