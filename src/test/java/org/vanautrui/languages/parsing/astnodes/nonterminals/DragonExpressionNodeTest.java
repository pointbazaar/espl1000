package org.vanautrui.languages.parsing.astnodes.nonterminals;

import org.junit.Test;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonExpressionNode;

public class DragonExpressionNodeTest {

    @Test
    public void test_simple_expression() throws Exception {

        DragonTokenList list = new DragonTokenList();
        list.add(new IntegerNonNegativeConstantToken(4));
        DragonExpressionNode expr = new DragonExpressionNode(list);
    }

    @Test
    public void test_variable_name_expression() throws Exception {

        DragonTokenList list = new DragonTokenList();
        list.add(new IdentifierToken("x"));
        DragonExpressionNode expr = new DragonExpressionNode(list);
    }


}
