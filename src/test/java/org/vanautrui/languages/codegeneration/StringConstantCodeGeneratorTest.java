package org.vanautrui.languages.codegeneration;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;

import static org.vanautrui.languages.codegeneration.CodeGeneratorTestUtils.compile_and_run_one_class_for_testing;

public class StringConstantCodeGeneratorTest {

    @Test
    public void test_can_return_multiline_string()throws Exception{
        String source="public class MainTest2663 { public Int main(){ print(\"h\nh\");  return 0;}  }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest2663");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("h\nh",IOUtils.toString(pr.getInputStream()));
    }

}
