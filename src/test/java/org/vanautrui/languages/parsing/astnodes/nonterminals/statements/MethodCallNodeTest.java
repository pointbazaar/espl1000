package org.vanautrui.languages.parsing.astnodes.nonterminals.statements;

import org.junit.Test;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.MethodCallNode;

public class MethodCallNodeTest {

    @Test
    public void test1() throws Exception {
        DragonTokenList list = new DragonTokenList();
        list.add(new IdentifierToken("main"));
        list.add(new SymbolToken("("));
        list.add(new SymbolToken(")"));
        list.add(new SymbolToken(";"));

        MethodCallNode call = new MethodCallNode(list);
    }

    @Test
    public void test2() throws Exception {
        DragonTokenList list = new DragonTokenList();
        list.add(new IdentifierToken("main"));
        list.add(new SymbolToken("("));
        list.add(new IntegerNonNegativeConstantToken(4));
        list.add(new SymbolToken(")"));
        list.add(new SymbolToken(";"));

        MethodCallNode call = new MethodCallNode(list);
    }

    @Test
    public void test3() throws Exception {
        DragonTokenList list = new DragonTokenList();
        list.add(new IdentifierToken("main"));
        list.add(new SymbolToken("("));
        list.add(new IdentifierToken("x"));
        list.add(new SymbolToken(")"));
        list.add(new SymbolToken(";"));

        MethodCallNode call = new MethodCallNode(list);
    }
}
