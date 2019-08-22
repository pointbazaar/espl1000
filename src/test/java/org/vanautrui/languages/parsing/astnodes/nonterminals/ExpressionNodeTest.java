package org.vanautrui.languages.parsing.astnodes.nonterminals;

import org.junit.Test;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.ExpressionNode;

public class ExpressionNodeTest {

    @Test
    public void test_simple_expression() throws Exception {

        DragonTokenList list = new DragonTokenList();
        list.add(new IntegerNonNegativeConstantToken(4));
        ExpressionNode expr = new ExpressionNode(list);
    }

    @Test
    public void test_variable_name_expression() throws Exception {

        DragonTokenList list = new DragonTokenList();
        list.add(new IdentifierToken("x"));
        ExpressionNode expr = new ExpressionNode(list);
    }


}
