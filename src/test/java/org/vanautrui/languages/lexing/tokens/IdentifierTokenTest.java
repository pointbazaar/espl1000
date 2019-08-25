package org.vanautrui.languages.lexing.tokens;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.lexing.collections.CharacterList;
import org.vanautrui.languages.lexing.Lexer;
import org.vanautrui.languages.lexing.collections.TokenList;

public class IdentifierTokenTest {

    @Test
    public void test1() throws Exception {

        CharacterList list = new CharacterList("main { pub");

        IdentifierToken identifierToken = new IdentifierToken(list);

        Assert.assertEquals("main", identifierToken.getContents());
    }

    @Test
    public void test_accept_underscore_identifier() throws Exception {

        String source = "vector <string> arg_ls;";

        Lexer lexer = new Lexer();
        TokenList tokenList = lexer.lexCodeTestMode(source);

        Assert.assertTrue(tokenList.get(4) instanceof IdentifierToken);

        Assert.assertEquals("arg_ls", tokenList.get(4).getContents());
    }

    @Test
    public void test_not_accept_uppercase()throws Exception{
        CharacterList list = new CharacterList("Int x;");

        try {
            IdentifierToken identifierToken = new IdentifierToken(list);
            Assert.fail();
        }catch (Exception e){
            //pass
        }

    }
}
