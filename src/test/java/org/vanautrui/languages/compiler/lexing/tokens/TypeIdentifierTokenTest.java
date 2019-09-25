package org.vanautrui.languages.compiler.lexing.tokens;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;

public class TypeIdentifierTokenTest {

    @Test
    public void test_simple_type() throws Exception {
        CharacterList list = new CharacterList("PInt");

        TypeIdentifierToken tk = new TypeIdentifierToken(list);

        Assert.assertEquals("PInt",tk.getContents());
    }

    @Test
    public void test_array_type() throws Exception {
        CharacterList list = new CharacterList("[PInt]");

        TypeIdentifierToken tk = new TypeIdentifierToken(list);

        Assert.assertEquals("[PInt]",tk.getContents());
    }

    @Test
    public void test_parse_type_identifier_token() throws Exception {

        CharacterList list = new CharacterList("PInt x;");

        TypeIdentifierToken identifierToken = new TypeIdentifierToken(list);

        Assert.assertEquals("PInt", identifierToken.getContents());
    }

    @Test
    public void test_not_accept_lowercase() throws Exception {

        CharacterList list = new CharacterList("pint x;");

        try {
            TypeIdentifierToken identifierToken = new TypeIdentifierToken(list);
            Assert.fail();
        }catch (Exception e){
            //pass
        }
    }
}
