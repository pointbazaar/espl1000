package org.vanautrui.languages.compiler.vmcodegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Ignore;
import org.junit.Test;

import static org.vanautrui.languages.CodeGeneratorTestUtils.compile_and_run_program_for_testing_with_cmd_args;

public class MethodCallStatementCodeGeneratorTest {

    @Test
    public void test_putchar_newline_works()throws Exception{
        String source="public namespace MainTest2331 { public ()~>PInt main{ putchar('\\n'); return 0;}  }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest2331",new String[0]);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("\n",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_call_other_method_with_argument()throws Exception{
        String faculty_methd="public (PInt n)->PInt faculty{ if(n!=1) { return (n*faculty(n-1));} return 1; }";
        String source="public namespace MainTest231 { public ()~>PInt main{ putdigit(faculty(3)); return 0;} "+faculty_methd+" }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest231",new String[0]);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("6",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_call_subroutine_argument()throws Exception{
        String method1="public (PInt n)->PInt id{  return n; }";
        String method2="public (((PInt)~>PInt) subr1)->PInt subr{  return subr1(2); }";
        String source="public namespace MainTest23138 { public ()~>PInt main{ putdigit(subr(id)); return 0;} "+method1+method2+" }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest23138",new String[0]);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("2",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_call_other_method_with_multiple_argument()throws Exception{
        String subr="public (PInt a,PInt b)~>PInt subr{ putdigit(a); putdigit(b); return 0; }";
        String source="public namespace MainTest2311 { public ()~>PInt main{ subr(1,2); return 0;} "+subr+" }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest2311",new String[0]);

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("12",IOUtils.toString(pr.getInputStream()));
    }

    @Ignore
    @Test
    public void test_can_access_commandline_arguments()throws Exception{

        String source="public namespace MainTest2313 { public ([String] args)~>PInt main{ putdigit(args[0]); return 0;} }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest2313",new String[]{"7"});

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("7",IOUtils.toString(pr.getInputStream()));
    }
}
