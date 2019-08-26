package org.vanautrui.languages.codegeneration;

import org.apache.commons.io.IOUtils;
import org.junit.Assert;
import org.junit.Test;

import static org.vanautrui.languages.codegeneration.CodeGeneratorTestUtils.compile_and_run_one_class_for_testing;

public class AssignmentStatementCodeGeneratorTest {


    @Test
    public void test_can_read_from_array()throws Exception{
        String source="public class MainTest333{" +
                "\tpublic Int main(){" +
                "\t\tarr=[1,2];" +
                "\t\tx=arr[0];" +
                "\t\tprint(x);" +
                "\t return 0;}" +
                "}";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest333");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("1",IOUtils.toString(pr.getInputStream()));
    }


    @Test
    public void test_can_write_into_array()throws Exception{
        String source="public class MainTest454{" +
                "\tpublic Int main(){" +
                "\t\tarr=[1,2];" +
                "\t\tarr[0]=2;" +
                "\t\tprint(arr[0]);" +
                "\treturn 0;}" +
                "}";
        Process pr = compile_and_run_one_class_for_testing(source,"MainTest454");

        Assert.assertEquals(0,pr.exitValue());
        Assert.assertEquals("2",IOUtils.toString(pr.getInputStream()));
    }
}
