package org.vanautrui.languages.compiler.vmcodegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Ignore;
import org.junit.Test;

import static org.vanautrui.languages.CodeGeneratorTestUtils.compile_and_run_program_for_testing_with_cmd_args;

public final class MethodCallStatementCodeGeneratorTest {

    @Test
    public void test_putchar_newline_works()throws Exception{
        final String source="fn main ()~>PInt { putchar('\\n'); return 0;}  ";
        final Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest2331",new String[0]);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("\n",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_call_other_method_with_argument()throws Exception{
        final String faculty_methd="fn faculty (PInt n)->PInt { if( n != 1) { return ( n * faculty( n - 1 ));} return 1; }";
        final String source="fn main ()~>PInt { putdigit( faculty( 3 ) ); return 0;} "+faculty_methd+" ";
        final Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest231",new String[0]);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("6",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_call_subroutine_argument()throws Exception{
        //https://stackoverflow.com/questions/9380363/resolving-circular-dependencies-by-linking-the-same-library-twice
        //apparently the order of linking problem can be solved by linking the same library twice? idk.
        //this is just general, not just related to this test

        final String method1="fn id (PInt n)->PInt {  return n; }";
        final String method2="fn subr (((PInt)~>PInt) subr1)->PInt {  return subr1(2); }";
        final String source="fn main ()~>PInt { putdigit(subr(id)); return 0;} "+method1+method2+" ";
        final Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest23138",new String[0]);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("2",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_call_other_method_with_multiple_argument()throws Exception{
        final String subr="fn subr (PInt a,PInt b)~>PInt { putdigit(a); putdigit(b); return 0; }";
        final String source="fn main ()~>PInt { subr( 1 , 2 ); return 0;} "+subr+" ";
        final Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest2311",new String[0]);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("12",IOUtils.toString(pr.getInputStream()));
    }

    @Ignore
    @Test
    public void test_can_access_commandline_arguments()throws Exception{

        final String source="fn main ([String] args)~>PInt { putdigit(args[0]); return 0;} ";
        final Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest2313",new String[]{"7"});

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("7",IOUtils.toString(pr.getInputStream()));
    }
}
