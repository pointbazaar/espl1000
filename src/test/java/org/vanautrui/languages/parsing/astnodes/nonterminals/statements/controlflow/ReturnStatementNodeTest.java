package org.vanautrui.languages.parsing.astnodes.nonterminals.statements.controlflow;

import org.junit.Test;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;

public class ReturnStatementNodeTest {

    @Test
    public void test1() throws Exception {
        TokenList list = new TokenList();
        list.add(new KeywordToken("return"));

        list.add(new SymbolToken("("));

	list.add(new OperatorToken("-"));
        list.add(new IntegerNonNegativeConstantToken(5));

        list.add(new SymbolToken(")"));

        list.add(new SymbolToken(";"));

        ReturnStatementNode returnStatementNode = new ReturnStatementNode(list);
    }

    @Test
    public void test2() throws Exception {
        TokenList list = new TokenList();
        list.add(new KeywordToken("return"));

        list.add(new SymbolToken("("));

	list.add(new OperatorToken("-"));
        list.add(new IntegerNonNegativeConstantToken(5));

        list.add(new SymbolToken(")"));

        list.add(new OperatorToken("*"));

        list.add(new IdentifierToken("n"));

        list.add(new SymbolToken(";"));

        ReturnStatementNode returnStatementNode = new ReturnStatementNode(list);

        System.out.println(returnStatementNode.toSourceCode());
    }

    @Test
    public void test3() throws Exception {
        TokenList list = new TokenList();
        list.add(new KeywordToken("return"));

        list.add(new IdentifierToken("arr"));

        list.add(new SymbolToken("["));
        list.add(new IntegerNonNegativeConstantToken(0));

        list.add(new SymbolToken("]"));

        list.add(new SymbolToken(";"));

        ReturnStatementNode returnStatementNode = new ReturnStatementNode(list);
    }
}
