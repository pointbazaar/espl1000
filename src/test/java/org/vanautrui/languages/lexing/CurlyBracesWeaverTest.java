package org.vanautrui.languages.lexing;

import org.junit.Assert;
import org.junit.Test;

public class CurlyBracesWeaverTest {

    @Test
    public void testWeave() throws Exception{

        String source="";

        String line1 = "line1";
        String line2 = "    line2";
        String line3 = "        line3";
        String line4 = "";

        source=line1+"\n"+line2+"\n"+line3+"\n"+line4+"\n";

        String result = CurlyBracesWeaver.weave_scoping_curly_braces_and_remove_newlines(source);

        Assert.assertEquals("line1 { line2 { line3 }  } ",result);
    }
}
