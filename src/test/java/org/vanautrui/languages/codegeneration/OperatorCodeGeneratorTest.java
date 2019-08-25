package org.vanautrui.languages.codegeneration;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;

import static org.vanautrui.languages.codegeneration.JavaByteCodeGeneratorTest.compile_and_run_one_class_for_testing;

public class OperatorCodeGeneratorTest {
	//this class tests that the 
	//Dragon Operator Code Generator generates correct operator byte codes

    @Test
    public void test_can_add_Float()throws Exception{
        String source="public class MainTest2777 { public Int main(){ print(1.0+2.0); return 0;} }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest2777");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("3.0",IOUtils.toString(pr.getInputStream()));
    }

	@Test
    public void test_can_sub_Float()throws Exception{
        String source="public class MainTest2775 { public Int main(){ print(1.0-2.0); return 0;} }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest2775");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("-1.0",IOUtils.toString(pr.getInputStream()));
    }

	@Test
    public void test_can_multiply_Float()throws Exception{
        String source="public class MainTest2774 { public Int main(){ print(5.0*2.0); return 0;} }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest2774");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("10.0",IOUtils.toString(pr.getInputStream()));
    }

	@Test
    public void test_can_div_Float()throws Exception{
        String source="public class MainTest2773 { public Int main(){ print(3.0/2.0); return 0; } }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest2773");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1.5",IOUtils.toString(pr.getInputStream()));
    }

	@Test
    public void test_can_add_Int()throws Exception{
        String source="public class MainTest6777 { public Int main(){ print(1/2); return 0;} }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest6777");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("0",IOUtils.toString(pr.getInputStream()));
    }

	@Test
    public void test_can_sub_Int()throws Exception{
        String source="public class MainTest6775 { public Int main(){ print(1-2); return 0; } }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest6775");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("-1",IOUtils.toString(pr.getInputStream()));
    }

	@Test
    public void test_can_multiply_Int()throws Exception{
        String source="public class MainTest6774 { public Int main(){ print(5*2); return 0;} }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest6774");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("10",IOUtils.toString(pr.getInputStream()));
    }

	@Test
    public void test_can_div_Int()throws Exception{
        String source="public class MainTest6773 { public Int main(){ print(3/2); return 0;} }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest6773");
        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }
}
