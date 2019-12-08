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
        //TODO:
        //the problem with this test is, the compiler generates dracovm insructions for integer comparison
        //it should generate dracovm instructions for float comparisons

        String source=" public ()~>PInt main{ x=1.0+2.0; if( (x>2.9) && (x<3.1)  ){ putdigit(1); } return 0;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest2777",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_sub_Float()throws Exception{
        String source=" public ()~>PInt main{ x=1.0-2.0; if(x < 0.0){ putdigit(1); } return 0;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest2775",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }


    @Test
    public void test_can_multiply_Float()throws Exception{
        String source=" public ()~>PInt main{ f=5.0*2.0; if( (f<10.1) && (f>9.9) ){ putdigit(1); } return 0;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest2774",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_div_Float()throws Exception{
        String source=" public ()~>PInt main{ f=3.0/2.0; if( (f > 1.4) && (f < 1.6)){ putdigit(1); } return 0; } ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest2773",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

	@Test
    public void test_can_add_Int()throws Exception{
        String source=" public ()~>PInt main{ putdigit(1+2); return 0;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest6777",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("3",IOUtils.toString(pr.getInputStream()));
    }

	@Test
    public void test_can_sub_Int()throws Exception{
        String source=" public ()~>PInt main{ putdigit(2-1); return 0; } ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest6775",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

	@Test
    public void test_can_multiply_Int()throws Exception{
        String source=" public ()~>PInt main{ putdigit(3*2); return 0;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest6774",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("6",IOUtils.toString(pr.getInputStream()));
    }

	  @Test
    public void test_can_div_Int()throws Exception{
        String source=" public ()~>PInt main{ putdigit(3/2); return 0;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest6773",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_modulo_Int()throws Exception{
        String source="public ()~>PInt main{ putdigit(3 % 2); return 0;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest67373",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_bitshift_1()throws Exception{
        String source=" public ()~>PInt main{ putdigit(1 << 1); return 0;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest673733",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("2",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_bitshift_2()throws Exception{
        final String source=" public ()~>PInt main{ putdigit(2 >> 1); return 0;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest673734",new String[0]);
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_exp_int()throws Exception{
        //tests the '^' operator for exponentiation
        //, for calculating exponentials

        final String source=" public ()~>PInt main{ return (2 ^ 3); } ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest6737341",new String[0]);
        Assert.assertEquals(8,pr.exitValue());
        Assert.assertEquals("",IOUtils.toString(pr.getInputStream()));
    }
}
