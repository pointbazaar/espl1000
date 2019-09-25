package org.vanautrui.languages.compiler.vmcodegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;

import static org.vanautrui.languages.CodeGeneratorTestUtils.compile_and_run_program_for_testing;

public class OperatorCodeGeneratorTest {
    //https://godbolt.org/

	//this class tests that the 
	//Dragon Operator Code Generator generates correct operator byte codes

    //TODO: re-enable the Float specific tests later on when the compiler is able to pass them

    public void test_can_add_Float()throws Exception{
        String source="public class MainTest2777 { public PInt main(){ print(1.0+2.0); return 0;} }";
        Process pr = compile_and_run_program_for_testing(source,"MainTest2777");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("3.0",IOUtils.toString(pr.getInputStream()));
    }

    public void test_can_sub_Float()throws Exception{
        String source="public class MainTest2775 { public PInt main(){ print(1.0-2.0); return 0;} }";
        Process pr = compile_and_run_program_for_testing(source,"MainTest2775");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("-1.0",IOUtils.toString(pr.getInputStream()));
    }


    public void test_can_multiply_Float()throws Exception{
        String source="public class MainTest2774 { public PInt main(){ print(5.0*2.0); return 0;} }";
        Process pr = compile_and_run_program_for_testing(source,"MainTest2774");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("10.0",IOUtils.toString(pr.getInputStream()));
    }


    public void test_can_div_Float()throws Exception{
        String source="public class MainTest2773 { public PInt main(){ print(3.0/2.0); return 0; } }";
        Process pr = compile_and_run_program_for_testing(source,"MainTest2773");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1.5",IOUtils.toString(pr.getInputStream()));
    }



	@Test
    public void test_can_add_Int()throws Exception{
        String source="public class MainTest6777 { public PInt main(){ putdigit(1+2); return 0;} }";
        Process pr = compile_and_run_program_for_testing(source,"MainTest6777");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("3",IOUtils.toString(pr.getInputStream()));
    }

	@Test
    public void test_can_sub_Int()throws Exception{
        String source="public class MainTest6775 { public PInt main(){ putdigit(2-1); return 0; } }";
        Process pr = compile_and_run_program_for_testing(source,"MainTest6775");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

	@Test
    public void test_can_multiply_Int()throws Exception{
        String source="public class MainTest6774 { public PInt main(){ putdigit(3*2); return 0;} }";
        Process pr = compile_and_run_program_for_testing(source,"MainTest6774");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("6",IOUtils.toString(pr.getInputStream()));
    }

	  @Test
    public void test_can_div_Int()throws Exception{
        String source="public class MainTest6773 { public PInt main(){ putdigit(3/2); return 0;} }";
        Process pr = compile_and_run_program_for_testing(source,"MainTest6773");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_modulo_Int()throws Exception{
        String source="public class MainTest67373 { public PInt main(){ putdigit(3 % 2); return 0;} }";
        Process pr = compile_and_run_program_for_testing(source,"MainTest67373");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }
}
