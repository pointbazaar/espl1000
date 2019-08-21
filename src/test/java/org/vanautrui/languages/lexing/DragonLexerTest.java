package org.vanautrui.languages.lexing;

import org.junit.Test;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.*;

import static org.junit.Assert.*;

public class DragonLexerTest {

    DragonLexer lexer = new DragonLexer();


    @Test
    public void test_can_see_access_modifiers() throws Exception {
        DragonTokenList token = lexer.lexCodeWithoutComments("public");
        DragonTokenList token2 = lexer.lexCodeWithoutComments("private");

        assertTrue(token.get(0) instanceof AccessModifierToken);
        assertTrue(token2.get(0) instanceof AccessModifierToken);

        assertTrue(((AccessModifierToken) token.get(0)).is_public);
        assertFalse(((AccessModifierToken) token2.get(0)).is_public);
    }

    @Test
    public void test_can_see_line_with_semicolon() throws Exception{
        DragonTokenList tokens = lexer.lexCodeWithoutComments("private char x;");

        //System.out.println(tokens.toSourceCodeFragment());
        //System.out.println(tokens.toString());

        assertTrue(tokens.get(0) instanceof AccessModifierToken);

        assertTrue(tokens.get(2) instanceof IdentifierToken);

        assertTrue(tokens.get(3) instanceof SymbolToken);
    }

    @Test
    public void test_can_see_line_with_operators()throws Exception{
        DragonTokenList tokens = lexer.lexCodeWithoutComments("x+=x;");

        //System.out.println(tokens.toSourceCodeFragment());
        //System.out.println(tokens.toString());

        assertTrue(tokens.get(0) instanceof IdentifierToken);

        assertTrue(tokens.get(1) instanceof OperatorToken);

        assertTrue(tokens.get(2) instanceof IdentifierToken);
    }

    @Test
    public void test_lexes_returnstatement_favorably()throws Exception{
        DragonTokenList tokens = lexer.lexCodeWithoutComments("return (-5)*n;");

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
        DragonTokenList tokens=lexer.lexCodeWithoutComments("return (n*faculty(n-1));");

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
}
