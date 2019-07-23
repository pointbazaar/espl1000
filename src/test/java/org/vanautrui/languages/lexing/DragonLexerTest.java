package org.vanautrui.languages.lexing;

import org.junit.Test;
import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.lexing.tokens.AccessModifierToken;
import org.vanautrui.languages.parsing.DragonTokenList;

import java.util.List;

import static org.junit.Assert.*;

public class DragonLexerTest {

    DragonLexer lexer=new DragonLexer();



    @Test
    public void test_can_see_access_modifiers() throws Exception{
        DragonTokenList token = lexer.lexCodeWithoutComments("public");
        DragonTokenList token2 = lexer.lexCodeWithoutComments("private");

        assertTrue(token.get(0) instanceof AccessModifierToken);
        assertTrue(token2.get(0) instanceof AccessModifierToken);

        assertTrue(((AccessModifierToken) token.get(0)).is_public);
        assertFalse(((AccessModifierToken)token2.get(0)).is_public);
    }
}
