package org.vanautrui.languages.compiler.vmcodegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.vanautrui.languages.CodeGeneratorTestUtils.compile_and_run_program_for_testing;

public class WhileStatementCodeGeneratorTest {


    @Test
    public void test_can_while_statement()throws Exception{
        String source="public class MainTest22 { public ()~>PInt main{ i=0; while (i<3) { putchar('1'); i=i+1; } return 0;} }";
        Process pr = compile_and_run_program_for_testing(source,"MainTest22");

        assertEquals(0,pr.exitValue());
        assertEquals("111",IOUtils.toString(pr.getInputStream()));
    }


}
