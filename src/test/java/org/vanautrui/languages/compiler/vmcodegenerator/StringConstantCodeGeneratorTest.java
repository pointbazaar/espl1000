package org.vanautrui.languages.compiler.vmcodegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;

import static org.vanautrui.languages.CodeGeneratorTestUtils.compile_and_run_program_for_testing_with_cmd_args;

public final class StringConstantCodeGeneratorTest {

    //@Test
    public void test_can_return_multiline_string()throws Exception{
        String source="public namespace MainTest2663 { public ()~>PInt main{ print(\"h\nh\");  return 0;}  }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest2663",new String[0]);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("h\nh",IOUtils.toString(pr.getInputStream()));
    }

}
