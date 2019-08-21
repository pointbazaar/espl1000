package org.vanautrui.languages.parsing.astnodes.nonterminals.statements.controlflow;

import org.junit.Test;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonReturnStatementNode;

public class DragonReturnStatementNodeTest {

    @Test
    public void test1() throws Exception {
        DragonTokenList list = new DragonTokenList();
        list.add(new KeywordToken("return"));

        list.add(new SymbolToken("("));

        list.add(new IntegerNonNegativeConstantToken(-5));

        list.add(new SymbolToken(")"));

        list.add(new SymbolToken(";"));

        DragonReturnStatementNode returnStatementNode = new DragonReturnStatementNode(list);
    }

    @Test
    public void test2() throws Exception {
        DragonTokenList list = new DragonTokenList();
        list.add(new KeywordToken("return"));

        list.add(new SymbolToken("("));

        list.add(new IntegerNonNegativeConstantToken(-5));

        list.add(new SymbolToken(")"));

        list.add(new OperatorToken("*"));

        list.add(new IdentifierToken("n"));

        list.add(new SymbolToken(";"));

        DragonReturnStatementNode returnStatementNode = new DragonReturnStatementNode(list);

        System.out.println(returnStatementNode.toSourceCode());
    }
}
