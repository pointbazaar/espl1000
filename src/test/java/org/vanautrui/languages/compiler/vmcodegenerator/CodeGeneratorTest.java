package org.vanautrui.languages.compiler.vmcodegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.TestUtils;

import java.io.BufferedOutputStream;
import java.io.OutputStream;

public class CodeGeneratorTest {

    @Test
    public void test_can_compile_simple_helloworld()throws Exception{
        final String source="fn main ()~>PInt { putchar('h'); putchar('w'); return 0; } ";

        final Process pr = TestUtils.compileAndRunProgramForTesting(source,"MainTest100",new String[0],false);

        Assert.assertEquals(0,pr.exitValue());

        final String output = IOUtils.toString(pr.getInputStream());

        Assert.assertEquals("hw",output);
    }

    @Test
    public void test_can_compile_assignment_statements()throws Exception{
        final int x=3;
        //int x=55; //with this value it worked
        //if we put x=3; it does not work because it generates the wrong bytecodes

        //HYPOTHESIS: asm could maybe OR or AND the different arguments for IntInstr(BIPUSH,3)
        //and receive lconst_0 as a result
        //maybe it assumes we use iconst_0 or similar to push a low valued integer constant

        //investigation and experimentation showed that we indeed have to use iconst_0 for pushi a 0
        //on the stack. we cannot use the more general BIPUSH
        //maybe this has optimization reasons

        final String source="fn main ()~>PInt { x="+x+"; putdigit(x); return 0; } ";
        final Process pr = TestUtils.compileAndRunProgramForTesting(source,"MainTest3",new String[0],false);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals(x+"",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_compile_multiple_assignment_to_same_variable_add()throws Exception{

        final String source="fn main ()~>PInt { x=1; x=x+1; putdigit(x); return 0;} ";
        final Process pr = TestUtils.compileAndRunProgramForTesting(source,"MainTest4",new String[0],false);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("2",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_compile_multiple_assignment_to_same_variable_multiply()throws Exception{

        final String source="fn main ()~>PInt { x=2; x=x*2; putdigit(x); return 0; } ";
        final Process pr = TestUtils.compileAndRunProgramForTesting(source,"MainTest5",new String[0],false);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("4",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_compile_assignment_to_multiple_local_variables()throws Exception{

        final String source="fn main ()~>PInt { x=2; y=1; x=x+y; putdigit(x); return 0; } ";
        final Process pr = TestUtils.compileAndRunProgramForTesting(source,"MainTest6",new String[0],false);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("3",IOUtils.toString(pr.getInputStream()));
    }

    //@Test
    public void test_can_compile_input_and_output_very_basic()throws Exception{

        final String source="fn main ()~>PInt { x=readint();  putdigit(x); return 0;} ";
        final Process pr = TestUtils.compile_and_run_but_not_waitFor(source,"MainTest7",false);

        //give input to process
        OutputStream out = pr.getOutputStream();

        BufferedOutputStream out2 = new BufferedOutputStream(out);

        out2.write("2\n".getBytes());
        out2.flush();

        pr.waitFor();


        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("2",IOUtils.toString(pr.getInputStream()));
    }
}
