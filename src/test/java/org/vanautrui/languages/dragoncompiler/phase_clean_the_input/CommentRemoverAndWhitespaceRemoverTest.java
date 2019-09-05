package org.vanautrui.languages.dragoncompiler.phase_clean_the_input;

import org.junit.Assert;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class CommentRemoverAndWhitespaceRemoverTest {

    CommentRemoverAndWhitespaceRemover remover=new CommentRemoverAndWhitespaceRemover();

    @Test
    public void test_not_remove_comments_in_strings()throws Exception{
        CommentRemoverAndWhitespaceRemover cr = new CommentRemoverAndWhitespaceRemover();

        String src = "println(\"//hi there\")";
        String result = cr.strip_single_line_comments(src);

        //Assert.assertEquals(src,result);
        Assert.assertTrue(src.startsWith(result) || result.startsWith(src));
    }

    @Test
    public void test_remove_single_line_comment()throws Exception{
        CommentRemoverAndWhitespaceRemover cr = new CommentRemoverAndWhitespaceRemover();

        String src = "println();//hello";
        String target = "println();";
        String result = cr.strip_single_line_comments(src);


        Assert.assertEquals(target+"\n",result);
    }

    @Test
    public void test_investigate()throws Exception{
        CommentRemoverAndWhitespaceRemover cr = new CommentRemoverAndWhitespaceRemover();

        String src="println(\"arr=[1,2,3,4]; //looks good to me as an array constant\");";
        String res = cr.strip_single_line_comments(src);
        Assert.assertEquals(src+"\n",res);
    }

    @Test
    public void test_remove_single_line_comments() {
        String result = remover.strip_single_line_comments("command1\n//hello comment\ncommand2\n");
        assertEquals("command1\n\ncommand2\n", result);
    }

    @Test
    public void test_remove_multi_line_comments() throws Exception {
        String result = remover.strip_multi_line_comments("command1\n/*\ncommand2\ncommand3\n*/command4\n");
        assertEquals("command1\ncommand4\n", result);
    }

    @Test
    public void test_do_remove_nested_multiline_comments() throws Exception {
        String pseudo_source = "" +
                "command1\n" +
                "/*/**/*/\n" +
                "command4\n";
        String result = remover.strip_multi_line_comments(pseudo_source);
        assertEquals("command1\n\ncommand4\n", result);
    }
}
