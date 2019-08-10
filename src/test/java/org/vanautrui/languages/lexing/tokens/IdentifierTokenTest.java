package org.vanautrui.languages.lexing.tokens;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.lexing.CharacterList;
import org.vanautrui.languages.lexing.DragonLexer;
import org.vanautrui.languages.parsing.DragonTokenList;

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

        DragonLexer lexer = new DragonLexer();
        DragonTokenList dragonTokenList = lexer.lexCodeWithoutComments(source);

        Assert.assertTrue(dragonTokenList.get(4) instanceof IdentifierToken);

        Assert.assertEquals("arg_ls",dragonTokenList.get(4).getContents());
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
