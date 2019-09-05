package org.vanautrui.languages.compiler.parsing.astnodes.terminal;

import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.lexing.tokens.*;

public class FloatConstNodeTest {

    @Test
    public void test1() throws Exception {
        TokenList list = new TokenList();
        list.add(new OperatorToken("-"));
        list.add(new FloatNonNegativeConstantToken(new CharacterList("4.0")));

        FloatConstNode node = new FloatConstNode(list);
    }

	@Test
    public void test2() throws Exception {
        TokenList list = new TokenList();
        list.add(new FloatNonNegativeConstantToken(new CharacterList("4.0")));

        FloatConstNode node = new FloatConstNode(list);
    }
}
