package org.vanautrui.languages.parsing.astnodes.terminal;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.BoolConstantToken;

public class BoolConstantNodeTest {

    @Test
    public void test_parse_bool_constant_node() throws Exception {
        TokenList list = new TokenList();
        list.add(new BoolConstantToken(new CharacterList("true")));

        BoolConstantNode node = new BoolConstantNode(list);
        Assert.assertEquals(true,node.value);
    }
}
