package org.vanautrui.languages.parsing.astnodes.nonterminals;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.astnodes.nonterminal.ArrayConstantNode;

public class ArrayConstantNodeTest {

    @Test
    public void test_empty_array() throws Exception {

        TokenList list = new TokenList();
        list.add(new SymbolToken("["));
        list.add(new SymbolToken("]"));
        ArrayConstantNode arrayConstantNode = new ArrayConstantNode(list);
        Assert.assertEquals(0,arrayConstantNode.elements.size());
    }

    @Test
    public void test_simple_array() throws Exception {

        TokenList list = new TokenList();
        list.add(new SymbolToken("["));
        list.add(new IntegerNonNegativeConstantToken(1));
        list.add(new SymbolToken("]"));
        ArrayConstantNode arrayConstantNode = new ArrayConstantNode(list);
        Assert.assertEquals(1,arrayConstantNode.elements.size());
    }

    @Test
    public void test_array_multiple_elements() throws Exception {

        TokenList list = new TokenList();
        list.add(new SymbolToken("["));
        list.add(new IntegerNonNegativeConstantToken(1));
        list.add(new SymbolToken(","));
        list.add(new IntegerNonNegativeConstantToken(2));
        list.add(new SymbolToken("]"));
        ArrayConstantNode arrayConstantNode = new ArrayConstantNode(list);

        Assert.assertEquals(2,arrayConstantNode.elements.size());
    }
}
