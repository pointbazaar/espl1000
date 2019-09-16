package org.vanautrui.languages.compiler.codegeneration;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;

import static org.vanautrui.languages.CodeGeneratorTestUtils.compile_and_run_program_for_testing;

public class IfStatementCodeGeneratorTest {


    @Test
    public void test_can_if_statement()throws Exception{
        String source="public class MainTest23 { public Int main(){  if (1<2) { print(1); } return 0; } }";
        Process pr = compile_and_run_program_for_testing(source,"MainTest23");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_if_else_statement()throws Exception{
        String source="public class MainTest24 { public Int main(){  if (3<2) { print(\"3<2\"); } else { print(\"3>=2\"); } return 0;} }";
        Process pr = compile_and_run_program_for_testing(source,"MainTest24");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("3>=2",IOUtils.toString(pr.getInputStream()));
    }
}
