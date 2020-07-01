package org.vanautrui.languages.compiler.codegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;

import static org.vanautrui.languages.TestUtils.compileAndRunProgramForTesting;

public final class IfStatementCodeGeneratorTest {

    @Test
    public void test_can_if_statement()throws Exception{
        final String source="fn main ()~>PInt {  if (1<2) { putchar('1'); } return 0; } ";
        final Process pr = compileAndRunProgramForTesting(source,"MainTest23",new String[0],false);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_if_else_statement()throws Exception{
        final String source="fn main ()~>PInt {  if (3<2) { putchar('3'); } else { putchar('2'); } return 0;} ";
        final Process pr = compileAndRunProgramForTesting(source,"MainTest24",new String[0],false);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("2",IOUtils.toString(pr.getInputStream()));
    }
}
