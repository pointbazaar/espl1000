package org.vanautrui.languages.lexing;

import org.junit.Test;
import org.vanautrui.languages.lexing.tokens.AccessModifierToken;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.DragonTokenList;

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

        System.out.println(tokens.toSourceCodeFragment());
        System.out.println(tokens.toString());

        assertTrue(tokens.get(0) instanceof AccessModifierToken);

        assertTrue(tokens.get(2) instanceof IdentifierToken);

        assertTrue(tokens.get(3) instanceof SymbolToken);
    }
}
