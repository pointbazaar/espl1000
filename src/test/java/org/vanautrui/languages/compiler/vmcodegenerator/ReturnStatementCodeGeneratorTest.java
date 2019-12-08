package org.vanautrui.languages.compiler.vmcodegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;

import static org.vanautrui.languages.CodeGeneratorTestUtils.compile_and_run_program_for_testing_with_cmd_args;

public class ReturnStatementCodeGeneratorTest {


    @Test
    public void test_can_return_statement()throws Exception{
        String source=" public ()~>PInt main{ return subr(); return 0; } public ()~>PInt subr{return 3;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest266",new String[0]);

        Assert.assertEquals(3,pr.exitValue());
        //Assert.assertEquals("3",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_return_before_end_of_method()throws Exception{
        String source="public ()~>PInt main{ putchar('1'); return 0; putchar('2'); } ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest27",new String[0]);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }


	public void test_can_return_Float()throws Exception{
        String source=" public ()~>PInt main{ putfloat(subroutine()); return 0;} public ()~>Float subroutine{return 1.0;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest273",new String[0]);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1.0",IOUtils.toString(pr.getInputStream()));
	}

}
