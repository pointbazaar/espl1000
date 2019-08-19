package org.vanautrui.languages.lexing;

import org.junit.Test;
import org.vanautrui.languages.phase_clean_the_input.DragonCommentRemover;

import static org.junit.Assert.assertEquals;

public class DragonCommentRemoverTest {

    DragonCommentRemover remover = new DragonCommentRemover();

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
