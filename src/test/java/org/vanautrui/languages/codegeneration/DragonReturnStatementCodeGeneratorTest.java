package org.vanautrui.languages.codegeneration;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;

import static org.vanautrui.languages.codegeneration.JavaByteCodeGeneratorTest.compile_and_run_one_class_for_testing;

public class DragonReturnStatementCodeGeneratorTest {


    @Test
    public void test_can_return_statement()throws Exception{
        String source="public class MainTest26 { public Void main(){ print(subroutine()); } public String subroutine(){return \"subroutine\";} }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest26");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("subroutine",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_return_before_end_of_method()throws Exception{
        String source="public class MainTest27 { public Void main(){ print(\"1\"); return; print(\"2\"); } }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest27");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

}
