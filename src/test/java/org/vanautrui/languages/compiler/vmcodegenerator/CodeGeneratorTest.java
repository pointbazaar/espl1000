package org.vanautrui.languages.compiler.vmcodegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.CodeGeneratorTestUtils;

import java.io.BufferedOutputStream;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Paths;

public class CodeGeneratorTest {

    @Test
    public void test_can_compile_simple_helloworld()throws Exception{
        String source="public class MainTest100 { public Int main(){ println(\"Hello World!\"); return 0; }}";

        Process pr = CodeGeneratorTestUtils.compile_and_run_program_for_testing(source,"MainTest100");

        Assert.assertEquals(0,pr.exitValue());

        String output = IOUtils.toString(pr.getInputStream());

        Assert.assertEquals("Hello World!\n",output);
    }



    @Test
    public void test_can_compile_assignment_statements()throws Exception{
        int x=3;
        //int x=55; //with this value it worked
        //if we put x=3; it does not work because it generates the wrong bytecodes

        //HYPOTHESIS: asm could maybe OR or AND the different arguments for IntInstr(BIPUSH,3)
        //and receive lconst_0 as a result
        //maybe it assumes we use iconst_0 or similar to push a low valued integer constant

        //investigation and experimentation showed that we indeed have to use iconst_0 for pushi a 0
        //on the stack. we cannot use the more general BIPUSH
        //maybe this has optimization reasons

        String source="public class MainTest3 { public Int main(){ x="+x+"; println(x); return 0; } }";
        Process pr = CodeGeneratorTestUtils.compile_and_run_program_for_testing(source,"MainTest3");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals(x+"\n",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_compile_multiple_assignment_to_same_variable_add()throws Exception{

        String source="public class MainTest4 { public Int main(){ x=1; x=x+1; println(x); return 0;} }";
        Process pr = CodeGeneratorTestUtils.compile_and_run_program_for_testing(source,"MainTest4");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("2\n",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_compile_multiple_assignment_to_same_variable_multiply()throws Exception{

        String source="public class MainTest5 { public Int main(){ x=2; x=x*2; println(x); return 0; } }";
        Process pr = CodeGeneratorTestUtils.compile_and_run_program_for_testing(source,"MainTest5");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("4\n",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_compile_assignment_to_multiple_local_variables()throws Exception{

        String source="public class MainTest6 { public Int main(){ x=2; y=1; x=x+y; println(x); return 0; } }";
        Process pr = CodeGeneratorTestUtils.compile_and_run_program_for_testing(source,"MainTest6");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("3\n",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_compile_input_and_output_very_basic()throws Exception{

        String source="public class MainTest7 { public Int main(){ x=readln();  println(x); return 0;} }";
        Process pr = CodeGeneratorTestUtils.compile_and_run_but_not_waitFor(source,"MainTest7");

        //give input to process
        OutputStream out = pr.getOutputStream();

        BufferedOutputStream out2 = new BufferedOutputStream(out);

        out2.write("2\n".getBytes());
        out2.flush();

        pr.waitFor();

        Files.delete(Paths.get("MainTest7.class"));

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("2\n",IOUtils.toString(pr.getInputStream()));
    }
}
