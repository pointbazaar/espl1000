package org.vanautrui.languages.dragoncompiler.lexing;

import org.junit.Test;
import org.vanautrui.languages.dragoncompiler.lexing.utils.TokenList;
import org.vanautrui.languages.dragoncompiler.lexing.tokens.*;

import static org.junit.Assert.*;

public class LexerTest {

    Lexer lexer = new Lexer();


    @Test
    public void test_can_see_access_modifiers() throws Exception {
        TokenList token = lexer.lexCodeTestMode("public");
        TokenList token2 = lexer.lexCodeTestMode("private");

        assertTrue(token.get(0) instanceof AccessModifierToken);
        assertTrue(token2.get(0) instanceof AccessModifierToken);

        assertTrue(((AccessModifierToken) token.get(0)).is_public);
        assertFalse(((AccessModifierToken) token2.get(0)).is_public);
    }

    @Test
    public void test_can_see_line_with_semicolon() throws Exception{
        TokenList tokens = lexer.lexCodeTestMode("private char x;");

        assertTrue(tokens.get(0) instanceof AccessModifierToken);

        assertTrue(tokens.get(2) instanceof IdentifierToken);

        assertTrue(tokens.get(3) instanceof SymbolToken);
    }

    @Test
    public void test_can_see_line_with_operators()throws Exception{
        TokenList tokens = lexer.lexCodeTestMode("x+=x;");

        assertTrue(tokens.get(0) instanceof IdentifierToken);

        assertTrue(tokens.get(1) instanceof OperatorToken);

        assertTrue(tokens.get(2) instanceof IdentifierToken);
    }

    @Test
    public void test_lexes_returnstatement_favorably()throws Exception{
        TokenList tokens = lexer.lexCodeTestMode("return (-5)*n;");

        assertTrue(tokens.get(0) instanceof KeywordToken);

        assertTrue(tokens.get(1) instanceof SymbolToken);
        assertTrue(tokens.get(2) instanceof OperatorToken);
        assertTrue(tokens.get(3) instanceof IntegerNonNegativeConstantToken);
        assertTrue(tokens.get(4) instanceof SymbolToken);

        assertTrue(tokens.get(5) instanceof OperatorToken);

        assertTrue(tokens.get(6) instanceof IdentifierToken);

        assertTrue(tokens.get(7) instanceof SymbolToken);
    }

    @Test
    public void test_lexes_other_returnstatement()throws Exception{
        TokenList tokens=lexer.lexCodeTestMode("return (n*faculty(n-1));");

        //System.out.println(tokens.get(0));
        assertTrue(tokens.get(0) instanceof KeywordToken); //return

        assertTrue(tokens.get(1) instanceof SymbolToken); //(
        assertTrue(tokens.get(2) instanceof IdentifierToken); //n
        assertTrue(tokens.get(3) instanceof OperatorToken); //*

        assertTrue(tokens.get(4) instanceof IdentifierToken); //faculty

        assertTrue(tokens.get(5) instanceof SymbolToken); //(

        assertTrue(tokens.get(6) instanceof IdentifierToken); //n

        assertTrue(tokens.get(7) instanceof OperatorToken); //-

        assertTrue(tokens.get(8) instanceof IntegerNonNegativeConstantToken); //1

        assertTrue(tokens.get(9) instanceof SymbolToken); //)

        assertTrue(tokens.get(10) instanceof SymbolToken); //)

        assertTrue(tokens.get(11) instanceof SymbolToken); //;

        assertEquals(12,tokens.size());
    }

	@Test
	public void test_lexes_float_constant()throws Exception{
		TokenList ts = lexer.lexCodeTestMode("return 1.44;");

	assertTrue(ts.get(0) instanceof KeywordToken);

	assertTrue(ts.get(1) instanceof FloatNonNegativeConstantToken);
	}
}
