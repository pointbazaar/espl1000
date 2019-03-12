package org.vanautrui.languages.lexing;

import org.junit.Test;
import org.vanautrui.languages.model.tokens.AmandaToken;
import org.vanautrui.languages.model.tokens.AccessModifierToken;

import static org.junit.Assert.*;

public class AmandaLexerTest {

    AmandaLexer lexer=new AmandaLexer();

    @Test
    public void test_throws_exception_on_empty_token(){
        try{
            lexer.decide_which_token("");
            fail();
        }catch (Exception e){
            //pass
        }
    }

    @Test
    public void test_can_see_access_modifiers() throws Exception{
        AmandaToken token = lexer.decide_which_token("public");
        AmandaToken token2 = lexer.decide_which_token("private");

        assertTrue(token instanceof AccessModifierToken);
        assertTrue(token2 instanceof AccessModifierToken);

        assertTrue(((AccessModifierToken) token).is_public);
        assertFalse(((AccessModifierToken)token2).is_public);
    }
}
