package org.vanautrui.languages.compiler.vmcodegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;

import static org.vanautrui.languages.CodeGeneratorTestUtils.compile_and_run_program_for_testing;
import static org.vanautrui.languages.CodeGeneratorTestUtils.compile_and_run_program_for_testing_with_cmd_args;

public class MethodCallStatementCodeGeneratorTest {


    @Test
    public void test_can_call_other_method_with_argument()throws Exception{
        String faculty_methd="public PInt faculty(PInt n){ if(n!=1) { return (n*faculty(n-1));} return 1; }";
        String source="public class MainTest231 { public PInt main(){ putdigit(faculty(3)); return 0;} "+faculty_methd+" }";
        Process pr = compile_and_run_program_for_testing(source,"MainTest231");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("6",IOUtils.toString(pr.getInputStream()));
    }


    public void test_can_access_commandline_arguments()throws Exception{

        String source="public class MainTest2313 { public PInt main([String] args){ putdigit(args[0]); return 0;} }";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest2313",new String[]{"7"});

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("7",IOUtils.toString(pr.getInputStream()));
    }
}
