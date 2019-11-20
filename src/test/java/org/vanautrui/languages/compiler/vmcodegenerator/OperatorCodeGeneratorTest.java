package org.vanautrui.languages.compiler.vmcodegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;

import static org.vanautrui.languages.CodeGeneratorTestUtils.compile_and_run_program_for_testing_with_cmd_args;

public class OperatorCodeGeneratorTest {
    //https://godbolt.org/

	//this class tests that the 
	//Dragon Operator Code Generator generates correct operator byte codes

    //re-enable the Float specific tests later on when the compiler is able to pass them

    @Test
    public void test_can_add_Float()throws Exception{
        String source="public namespace MainTest2777 { public ()~>PInt main{ putfloat(1.0+2.0); return 0;} }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest2777",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("3.0",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_sub_Float()throws Exception{
        String source="public namespace MainTest2775 { public ()~>PInt main{ print(1.0-2.0); return 0;} }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest2775",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("-1.0",IOUtils.toString(pr.getInputStream()));
    }


    @Test
    public void test_can_multiply_Float()throws Exception{
        String source="public namespace MainTest2774 { public ()~>PInt main{ print(5.0*2.0); return 0;} }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest2774",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("10.0",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_div_Float()throws Exception{
        String source="public namespace MainTest2773 { public ()~>PInt main{ print(3.0/2.0); return 0; } }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest2773",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1.5",IOUtils.toString(pr.getInputStream()));
    }

	@Test
    public void test_can_add_Int()throws Exception{
        String source="public namespace MainTest6777 { public ()~>PInt main{ putdigit(1+2); return 0;} }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest6777",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("3",IOUtils.toString(pr.getInputStream()));
    }

	@Test
    public void test_can_sub_Int()throws Exception{
        String source="public namespace MainTest6775 { public ()~>PInt main{ putdigit(2-1); return 0; } }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest6775",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

	@Test
    public void test_can_multiply_Int()throws Exception{
        String source="public namespace MainTest6774 { public ()~>PInt main{ putdigit(3*2); return 0;} }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest6774",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("6",IOUtils.toString(pr.getInputStream()));
    }

	  @Test
    public void test_can_div_Int()throws Exception{
        String source="public namespace MainTest6773 { public ()~>PInt main{ putdigit(3/2); return 0;} }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest6773",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_modulo_Int()throws Exception{
        String source="public namespace MainTest67373 { public ()~>PInt main{ putdigit(3 % 2); return 0;} }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest67373",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_bitshift_1()throws Exception{
        String source="public namespace MainTest673733 { public ()~>PInt main{ putdigit(1 << 1); return 0;} }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest673733",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("2",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_bitshift_2()throws Exception{
        final String source="public namespace MainTest673734 { public ()~>PInt main{ putdigit(2 >> 1); return 0;} }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest673734",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_exp_int()throws Exception{
        //tests the '^' operator for exponentiation
        //, for calculating exponentials

        final String source="public namespace MainTest6737341 { public ()~>PInt main{ return (2 ^ 3); } }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest6737341",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("8",IOUtils.toString(pr.getInputStream()));
    }
}
