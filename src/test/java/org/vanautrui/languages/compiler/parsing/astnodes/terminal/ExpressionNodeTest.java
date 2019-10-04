package org.vanautrui.languages.compiler.parsing.astnodes.terminal;

import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;

public class ExpressionNodeTest {

    @Test
    public void test_simple_expression() throws Exception {

        TokenList list = new TokenList();
        list.add(new IntegerNonNegativeConstantToken(4));
        ExpressionNode expr = new ExpressionNode(list);
    }

    @Test
    public void test_variable_name_expression() throws Exception {

        TokenList list = new TokenList();
        list.add(new IdentifierToken("x"));
        ExpressionNode expr = new ExpressionNode(list);
    }

    @Test
    public void recognize_string_constant_expression()throws Exception{

        Lexer lexer = new Lexer();
        TokenList tokens = lexer.lexCodeTestMode("\"hello\"");
        //System.out.println(tokens.toString());

        ExpressionNode expr = new ExpressionNode(tokens);
    }
}
