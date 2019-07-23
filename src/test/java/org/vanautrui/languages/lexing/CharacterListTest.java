package org.vanautrui.languages.lexing;

import org.junit.Assert;
import org.junit.Test;

public class CharacterListTest {

    @Test
    public void test_this_cuty_class()throws Exception{

        CharacterList list = new CharacterList("hi there");

        Assert.assertTrue(list.startsWith("hi"));



        Assert.assertEquals("hi",list.getLimitedString(2));

        list.consumeTokens(3);

        Assert.assertEquals("there".length(),list.size());

        Assert.assertEquals("there",list.getLimitedString(5));

        try{
            list.consumeTokens(1000);
            Assert.fail("should have thrown an exception on the attempt to consume so many tokens which aint there");
        }catch (Exception e){
            //pass
        }


    }
}
