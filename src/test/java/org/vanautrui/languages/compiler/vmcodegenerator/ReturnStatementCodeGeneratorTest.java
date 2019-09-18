package org.vanautrui.languages.compiler.vmcodegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;

import static org.vanautrui.languages.CodeGeneratorTestUtils.compile_and_run_program_for_testing;

public class ReturnStatementCodeGeneratorTest {


    @Test
    public void test_can_return_statement()throws Exception{
        String source="public class MainTest266 { public Int main(){ return subr(); return 0; } public Int subr(){return 3;} }";
        Process pr = compile_and_run_program_for_testing(source,"MainTest266");

        Assert.assertEquals(3,pr.exitValue());
        //Assert.assertEquals("3",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_return_before_end_of_method()throws Exception{
        String source="public class MainTest27 { public Int main(){ putchar('1'); return 0; putchar('2'); } }";
        Process pr = compile_and_run_program_for_testing(source,"MainTest27");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }


	public void test_can_return_Float()throws Exception{
        String source="public class MainTest273 { public Int main(){ putfloat(subroutine()); return 0;} public Float subroutine(){return 1.0;} }";
        Process pr = compile_and_run_program_for_testing(source,"MainTest273");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1.0",IOUtils.toString(pr.getInputStream()));
	}

}
