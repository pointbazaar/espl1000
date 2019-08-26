package org.vanautrui.languages.parsing.astnodes.terminal;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.CharConstantToken;

public class CharConstantNodeTest {

    @Test
    public void test_parse_char_constant_node() throws Exception {
        TokenList list = new TokenList();
        list.add(new CharConstantToken(new CharacterList("'h'")));

        CharConstantNode node = new CharConstantNode(list);
        Assert.assertEquals('h',node.content);
    }
}
