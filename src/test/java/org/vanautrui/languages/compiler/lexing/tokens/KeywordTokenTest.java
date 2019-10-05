package org.vanautrui.languages.compiler.lexing.tokens;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;

import static org.junit.Assert.assertEquals;

public class KeywordTokenTest {

    @Test
    public void test4() throws Exception {
        CharacterList list = new CharacterList("namespace");
        KeywordToken k = new KeywordToken(list);

        assertEquals(0,list.size());
        assertEquals("namespace", k.getContents());
    }

    @Test
    public void test3() throws Exception {
        CharacterList list = new CharacterList("struct");
        KeywordToken k = new KeywordToken(list);

        assertEquals(0,list.size());
        assertEquals("struct", k.getContents());
    }

    @Test
    public void test1() throws Exception {
        CharacterList list = new CharacterList("return;");
        KeywordToken k = new KeywordToken(list);


        assertEquals(1,list.size());
        assertEquals("return", k.getContents());
    }

    @Test
    public void test2() throws Exception {
        CharacterList list = new CharacterList("returna");
        try {
            KeywordToken k = new KeywordToken(list);
            Assert.fail();
        }catch (Exception e){
            //pass. this is an identifier
        }
        assertEquals("returna".length(),list.size());
    }
}