package org.vanautrui.languages.dragoncompiler.lexing.tokens;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.dragoncompiler.lexing.utils.CharacterList;

public class KeywordTokenTest {

    @Test
    public void test1() throws Exception {
        CharacterList list = new CharacterList("return;");
        KeywordToken k = new KeywordToken(list);


        Assert.assertEquals(1,list.size());
        Assert.assertEquals("return", k.getContents());
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
        Assert.assertEquals("returna".length(),list.size());
    }
}