package org.vanautrui.languages.compiler.vmcodegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;

import static org.vanautrui.languages.CodeGeneratorTestUtils.compile_and_run_program_for_testing;

public class AssignmentStatementCodeGeneratorTest {

    @Test
    public void test_can_assign_to_local_variable()throws Exception{
        //depends on loop statement,return statement already working

        String source="public class MainTest333{" +
                "\tpublic PInt main(){" +
                "\t\tx=3;" +
                "\t\tloop x {putchar('x');}" +
                "\t return 0;}" +
                "}";
        Process pr = compile_and_run_program_for_testing(source,"MainTest333");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("xxx",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_assign_to_local_variable_muliple_times()throws Exception{
        //depends on loop statement,return statement already working

        String source="public class MainTest333{" +
                "\tpublic PInt main(){" +
                "\t\tx=3;" +
                "\t\tx=2;" +
                "\t\tloop x {putchar('x');}" +
                "\t return 0;}" +
                "}";
        Process pr = compile_and_run_program_for_testing(source,"MainTest333");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("xx",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_read_from_array()throws Exception{
        String source="public class MainTest333{" +
                "\tpublic PInt main(){" +
                "\t\tarr=['A','B'];" +
                "\t\tx=arr[0];" +
                "\t\tputchar(x);" +
                "\t return 0;}" +
                "}";
        Process pr = compile_and_run_program_for_testing(source,"MainTest333");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("A",IOUtils.toString(pr.getInputStream()));
    }


    @Test
    public void test_can_write_into_array()throws Exception{
        String source="public class MainTest454{" +
                "\tpublic PInt main(){" +
                "\t\tarr=['a','b','c'];" +
                "\t\tarr[1]='d';" +
                "\t\tputchar(arr[1]);" +
                "\treturn 0;}" +
                "}";
        Process pr = compile_and_run_program_for_testing(source,"MainTest454");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("d",IOUtils.toString(pr.getInputStream()));
    }
}
