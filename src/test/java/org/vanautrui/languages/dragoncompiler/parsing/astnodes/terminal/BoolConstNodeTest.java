package org.vanautrui.languages.dragoncompiler.parsing.astnodes.terminal;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.dragoncompiler.lexing.utils.CharacterList;
import org.vanautrui.languages.dragoncompiler.lexing.utils.TokenList;
import org.vanautrui.languages.dragoncompiler.lexing.tokens.BoolConstantToken;

public class BoolConstNodeTest {

    @Test
    public void test_parse_bool_constant_node() throws Exception {
        TokenList list = new TokenList();
        list.add(new BoolConstantToken(new CharacterList("true")));

        BoolConstNode node = new BoolConstNode(list);
        Assert.assertEquals(true,node.value);
    }
}
