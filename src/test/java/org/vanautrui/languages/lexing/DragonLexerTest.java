package org.vanautrui.languages.lexing;

import org.junit.Test;
import org.vanautrui.languages.model.tokens.DragonToken;
import org.vanautrui.languages.model.tokens.AccessModifierToken;

import java.util.List;

import static org.junit.Assert.*;

public class DragonLexerTest {

    DragonLexer lexer=new DragonLexer();



    @Test
    public void test_can_see_access_modifiers() throws Exception{
        List<DragonToken> token = lexer.lexCodeWithoutComments("public");
        List<DragonToken> token2 = lexer.lexCodeWithoutComments("private");

        assertTrue(token.get(0) instanceof AccessModifierToken);
        assertTrue(token2.get(0) instanceof AccessModifierToken);

        assertTrue(((AccessModifierToken) token.get(0)).is_public);
        assertFalse(((AccessModifierToken)token2.get(0)).is_public);
    }
}
