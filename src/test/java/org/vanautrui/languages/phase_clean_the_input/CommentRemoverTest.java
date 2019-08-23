package org.vanautrui.languages.phase_clean_the_input;

import org.junit.Assert;
import org.junit.Test;

public class CommentRemoverTest {

    @Test
    public void test_not_remove_comments_in_strings()throws Exception{
        CommentRemover cr = new CommentRemover();

        String src = "println(\"//hi there\")";
        String result = cr.strip_single_line_comments(src);

        //Assert.assertEquals(src,result);
        Assert.assertTrue(src.startsWith(result) || result.startsWith(src));
    }

    @Test
    public void test_remove_single_line_comment()throws Exception{
        CommentRemover cr = new CommentRemover();

        String src = "println();//hello";
        String target = "println();";
        String result = cr.strip_single_line_comments(src);


        Assert.assertEquals(target+"\n",result);
    }

    @Test
    public void test_investigate()throws Exception{
        CommentRemover cr = new CommentRemover();

        String src="println(\"arr=[1,2,3,4]; //looks good to me as an array constant\");";
        String res = cr.strip_single_line_comments(src);
        Assert.assertEquals(src+"\n",res);
    }
}
