package org.vanautrui.languages.compiler.vmcodegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.vanautrui.languages.CodeGeneratorTestUtils.compile_and_run_program_for_testing_with_cmd_args;

public final class AssignmentStatementCodeGeneratorTest {

    @Test
    public void test_can_assign_to_local_variable()throws Exception{
        //depends on loop statement,return statement already working

        final String source=
                " ()~>PInt main{" +
                "   x=3;" +
                "   loop x {putchar('x');}" +
                "   return 0;" +
                "} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest333",new String[0]);

        assertEquals(0,pr.exitValue());
        assertEquals("xxx",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_assign_to_local_variable_muliple_times()throws Exception{
        //depends on loop statement,return statement already working

        String source=
                " ()~>PInt main{" +
                "   x=3;" +
                "   x=2;" +
                "   loop x {putchar('x');}" +
                " return 0;} " ;
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest333",new String[0]);

        assertEquals(0,pr.exitValue());
        assertEquals("xx",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_read_from_array()throws Exception{
        String source=
                " ()~>PInt main{" +
                "   arr=['A','B'];" +
                "   x=arr[0];" +
                "   putchar(x);" +
                " return 0;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest333",new String[0]);

        assertEquals(0,pr.exitValue());
        assertEquals("A",IOUtils.toString(pr.getInputStream()));
    }


    @Test
    public void test_can_write_into_array()throws Exception{
        String source=
                " ()~>PInt main{" +
                "   arr=['a','b','c'];" +
                "   arr[1]='d';" +
                "   putchar(arr[1]);" +
                "return 0;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest454",new String[0]);

        assertEquals(0,pr.exitValue());
        assertEquals("d",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_use_structs()throws Exception{
        String source=
                "struct MyStruct{PInt b}"+
                " ()~>PInt main{" +
                "   MyStruct a=new(1);" +
                "   a.b=1;" +
                "   putdigit(a.b);" +
                "return 0;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest954",new String[0]);

        assertEquals(0,pr.exitValue());
        assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_use_structs_nested()throws Exception{
        String source=
                "struct MyStruct{PInt b,MyStruct s}"+
                " ()~>PInt main{" +
                "   MyStruct a = new(2);" +
                "   a.s = new(2);"+
                "   a.s.b=1;" +
                "   putdigit(a.s.b);" +
                "return 0;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest9541",new String[0]);

        assertEquals(0,pr.exitValue());
        assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }
}
