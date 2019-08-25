package org.vanautrui.languages.parsing.astnodes.terminal;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;

public class VariableNodeTest {

    @Test
    public void test_parse_simple_variable() throws Exception {
        TokenList list = new TokenList();
        list.add(new IdentifierToken(new CharacterList("x")));

        VariableNode node = new VariableNode(list);
        Assert.assertEquals("x",node.name);
    }

    @Test
    public void test_parse_simple_indexed_variable() throws Exception {
        TokenList list = new TokenList();
        list.add(new IdentifierToken(new CharacterList("x")));
        list.add(new SymbolToken("["));
        list.add(new IntegerNonNegativeConstantToken(0));
        list.add(new SymbolToken("]"));


        VariableNode node = new VariableNode(list);
        Assert.assertEquals("x",node.name);
        Assert.assertTrue(node.indexOptional.isPresent());
        //TODO: test the index
    }
}
