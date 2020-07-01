package org.vanautrui.languages.compiler.codegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;

import static org.vanautrui.languages.TestUtils.compileAndRunProgramForTesting;

public final class ReturnStatementCodeGeneratorTest {


    @Test
    public void test_can_return_statement()throws Exception{
        final String source="fn main ()~>PInt { return subr(); return 0; }  fn subr ()~>PInt {return 3;} ";
        final Process pr = compileAndRunProgramForTesting(source,"MainTest266",new String[0],false);

        Assert.assertEquals(3,pr.exitValue());
        //Assert.assertEquals("3",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_return_before_end_of_method()throws Exception{
        final String source="fn main ()~>PInt { putchar('1'); return 0; putchar('2'); } ";
        final Process pr = compileAndRunProgramForTesting(source,"MainTest27",new String[0],false);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }


	public void test_can_return_Float()throws Exception{
        final String source="fn main ()~>PInt { putfloat(subroutine()); return 0;}  fn subroutine ()~>Float {return 1.0;} ";
        final Process pr = compileAndRunProgramForTesting(source,"MainTest273",new String[0],false);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1.0",IOUtils.toString(pr.getInputStream()));
	}

}
