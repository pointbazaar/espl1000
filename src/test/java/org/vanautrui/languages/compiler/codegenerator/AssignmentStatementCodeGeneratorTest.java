package org.vanautrui.languages.compiler.codegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.vanautrui.languages.TestUtils.compileAndRunProgramForTesting;

public final class AssignmentStatementCodeGeneratorTest {

    @Test
    public void test_can_assign_to_local_variable()throws Exception{
        //depends on loop statement,return statement already working

        final String source=
                "fn main ()~>PInt {" +
                "   x=3;" +
                "   loop x {putchar('x');}" +
                "   return 0;" +
                "} ";

        final Process pr =
            compileAndRunProgramForTesting(source,"MainTest3943833",new String[0],true);

        assertEquals(0,pr.exitValue());
        assertEquals("xxx",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_assign_to_local_variable_muliple_times()throws Exception{
        //depends on loop statement,return statement already working

        final String source=
                "fn main ()~>PInt {" +
                "   x=3;" +
                "   x=2;" +
                "   loop x {putchar('x');}" +
                " return 0;} " ;
        final Process pr = compileAndRunProgramForTesting(source,"MainTest3323843",new String[0],false);

        assertEquals(0,pr.exitValue());
        assertEquals("xx",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_read_from_array()throws Exception{
        final String source=
                "fn main ()~>PInt {" +
                "   arr=['A','B'];" +
                "   x=arr[0];" +
                "   putchar(x);" +
                " return 0;} ";
        final Process pr = compileAndRunProgramForTesting(source,"MainTest3983433",new String[0],false);

        assertEquals(0,pr.exitValue());
        assertEquals("A",IOUtils.toString(pr.getInputStream()));
    }


    @Test
    public void test_can_write_into_array()throws Exception{
        final String source=
                "fn main ()~>PInt {" +
                "   arr=['a','b','c'];" +
                "   arr[1]='d';" +
                "   putchar(arr[1]);" +
                "return 0;} ";
        final Process pr = compileAndRunProgramForTesting(source,"MainTest349454",new String[0],false);

        assertEquals(0,pr.exitValue());
        assertEquals("d",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_use_structs()throws Exception{
        final String source=
                "struct MyStruct{PInt b}"+
                "fn main ()~>PInt {" +
                "   MyStruct a=new(1);" +
                "   a.b=1;" +
                "   putdigit(a.b);" +
                "return 0;} ";
        final Process pr = compileAndRunProgramForTesting(source,"MainTest953434",new String[0],false);

        assertEquals(0,pr.exitValue());
        assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_use_structs_nested()throws Exception{
        final String source=
                "struct MyStruct{PInt b,MyStruct s}"+
                "fn main ()~>PInt {" +
                "   MyStruct a = new(2);" +
                "   a.s = new(2);"+
                "   a.s.b=1;" +
                "   putdigit(a.s.b);" +
                "return 0;} ";
        final Process pr = compileAndRunProgramForTesting(source,"MainTest943541",new String[0],false);

        assertEquals(0,pr.exitValue());
        assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }
}
