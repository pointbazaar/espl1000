package org.vanautrui.languages.codegeneration;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;

import static org.vanautrui.languages.codegeneration.JavaByteCodeGeneratorTest.compile_and_run_one_class_for_testing;

public class DragonMethodCallStatementCodeGeneratorTest {


    @Test
    public void test_can_call_other_method_with_argument()throws Exception{
        String faculty_methd="public Int faculty(Int n){ if(n!=1) { return (n*faculty(n-1));} return 1; }";
        String source="public class MainTest231 { public Void main(){ print(faculty(3)); } "+faculty_methd+" }";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest231");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("6",IOUtils.toString(pr.getInputStream()));
    }

}
