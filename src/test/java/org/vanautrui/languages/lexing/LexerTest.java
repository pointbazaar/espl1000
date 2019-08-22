package org.vanautrui.languages.lexing;

import org.junit.Test;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.*;

import static org.junit.Assert.*;

public class LexerTest {

    Lexer lexer = new Lexer();


    @Test
    public void test_can_see_access_modifiers() throws Exception {
        TokenList token = lexer.lexCodeWithoutComments("public");
        TokenList token2 = lexer.lexCodeWithoutComments("private");

        assertTrue(token.get(0) instanceof AccessModifierToken);
        assertTrue(token2.get(0) instanceof AccessModifierToken);

        assertTrue(((AccessModifierToken) token.get(0)).is_public);
        assertFalse(((AccessModifierToken) token2.get(0)).is_public);
    }

    @Test
    public void test_can_see_line_with_semicolon() throws Exception{
        TokenList tokens = lexer.lexCodeWithoutComments("private char x;");

        //System.out.println(tokens.toSourceCodeFragment());
        //System.out.println(tokens.toString());

        assertTrue(tokens.get(0) instanceof AccessModifierToken);

        assertTrue(tokens.get(2) instanceof IdentifierToken);

        assertTrue(tokens.get(3) instanceof SymbolToken);
    }

    @Test
    public void test_can_see_line_with_operators()throws Exception{
        TokenList tokens = lexer.lexCodeWithoutComments("x+=x;");

        //System.out.println(tokens.toSourceCodeFragment());
        //System.out.println(tokens.toString());

        assertTrue(tokens.get(0) instanceof IdentifierToken);

        assertTrue(tokens.get(1) instanceof OperatorToken);

        assertTrue(tokens.get(2) instanceof IdentifierToken);
    }

    @Test
    public void test_lexes_returnstatement_favorably()throws Exception{
        TokenList tokens = lexer.lexCodeWithoutComments("return (-5)*n;");

        //System.out.println(tokens.toSourceCodeFragment());
        //System.out.println(tokens.toString());

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
        TokenList tokens=lexer.lexCodeWithoutComments("return (n*faculty(n-1));");

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
    }

	@Test
	public void test_lexes_float_constant()throws Exception{
		TokenList ts = lexer.lexCodeWithoutComments("return 1.44;");

	assertTrue(ts.get(0) instanceof KeywordToken);

	assertTrue(ts.get(1) instanceof FloatNonNegativeConstantToken);
	}
}
