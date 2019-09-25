package org.vanautrui.languages.compiler.lexing.tokens.utils;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.tokens.KeywordToken;

public class ITokenTest {

    @Test
    public void test_tokenEquals() throws Exception{

        KeywordToken k1= new KeywordToken("return");
        KeywordToken k2= new KeywordToken("return");

        Assert.assertTrue(k1.tokenEquals(k2));
    }

    @Test
    public void test_tokenEquals2() throws Exception{

        KeywordToken k1= new KeywordToken("return");
        KeywordToken k2= new KeywordToken("while");

        Assert.assertFalse(k1.tokenEquals(k2));
    }
}
