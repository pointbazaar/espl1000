package org.vanautrui.languages.parsing.astnodes.terminal;

import org.junit.Test;
import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.*;

public class FloatConstantNodeTest {

    @Test
    public void test1() throws Exception {
        TokenList list = new TokenList();
        list.add(new OperatorToken("-"));
        list.add(new FloatNonNegativeConstantToken(new CharacterList("4.0")));

        FloatConstantNode node = new FloatConstantNode(list);
    }

	@Test
    public void test2() throws Exception {
        TokenList list = new TokenList();
        list.add(new FloatNonNegativeConstantToken(new CharacterList("4.0")));

        FloatConstantNode node = new FloatConstantNode(list);
    }
}
