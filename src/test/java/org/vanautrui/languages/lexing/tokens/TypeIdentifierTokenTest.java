package org.vanautrui.languages.lexing.tokens;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.lexing.utils.CharacterList;

public class TypeIdentifierTokenTest {

    @Test
    public void test_simple_type() throws Exception {
        CharacterList list = new CharacterList("Int");

        TypeIdentifierToken tk = new TypeIdentifierToken(list);

        Assert.assertEquals("Int",tk.getContents());
    }

    @Test
    public void test_array_type() throws Exception {
        CharacterList list = new CharacterList("[Int]");

        TypeIdentifierToken tk = new TypeIdentifierToken(list);

        Assert.assertEquals("[Int]",tk.getContents());
    }

    @Test
    public void test_parse_type_identifier_token() throws Exception {

        CharacterList list = new CharacterList("Int x;");

        TypeIdentifierToken identifierToken = new TypeIdentifierToken(list);

        Assert.assertEquals("Int", identifierToken.getContents());
    }

    @Test
    public void test_not_accept_lowercase() throws Exception {

        CharacterList list = new CharacterList("int x;");

        try {
            TypeIdentifierToken identifierToken = new TypeIdentifierToken(list);
            Assert.fail();
        }catch (Exception e){
            //pass
        }
    }
}
