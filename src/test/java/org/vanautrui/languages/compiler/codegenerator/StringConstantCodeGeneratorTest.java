package org.vanautrui.languages.compiler.codegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;

import static org.vanautrui.languages.TestUtils.compileAndRunProgramForTesting;

public final class StringConstantCodeGeneratorTest {

    //@Test
    public void test_can_return_multiline_string()throws Exception{
        final String source="fn main ()~>PInt { print(\"h\nh\");  return 0;}  ";
        final Process pr = compileAndRunProgramForTesting(source,"MainTest2663",new String[0],false);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("h\nh",IOUtils.toString(pr.getInputStream()));
    }

}
