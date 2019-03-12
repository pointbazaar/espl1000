package org.vanautrui.languages.lexing;

import org.junit.Assert;
import org.junit.Test;

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

    
}
