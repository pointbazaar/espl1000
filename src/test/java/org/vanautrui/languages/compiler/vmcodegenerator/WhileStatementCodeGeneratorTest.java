package org.vanautrui.languages.compiler.vmcodegenerator;

import org.apache.commons.io.IOUtils;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.vanautrui.languages.CodeGeneratorTestUtils.compile_and_run_program_for_testing_with_cmd_args;

public class WhileStatementCodeGeneratorTest {


    @Test
    public void test_can_while_statement()throws Exception{
        String source="  ()~>PInt main{ i=0; while (i<3) { putchar('1'); i=i+1; } return 0;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest22",new String[0]);

        assertEquals(0,pr.exitValue());
        assertEquals("111",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_while_statement_and_call_builtin_subroutines()throws Exception{
        String source=" ()~>PInt main{ [PInt] arr=new(2); i=0; while (i<len(arr)) { putchar('1'); i=i+1; } return 0;} ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest223",new String[0]);

        assertEquals(0,pr.exitValue());
        assertEquals("11",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_while_statement_and_call_builtin_subroutines_other_subroutine()throws Exception{

        final String printarr = "([PInt] arr)~>PInt printarr{ i=0; while (i<len(arr)) { putchar('1'); i=i+1; } return 0;}";

        final String main = " ()~>PInt main{ [PInt] arr=new(2); printarr(arr); return 0;}";

        final String source=" "+main+" "+printarr+" ";
        Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest223",new String[0]);

        assertEquals(0,pr.exitValue());
        assertEquals("11",IOUtils.toString(pr.getInputStream()));
    }

    @Test
    public void test_can_while_statement_and_call_builtin_subroutines_other_subroutine_and_assign()throws Exception{

        final String printarr = "([PInt] arr)~>PInt printarr{ i=0; while (i<len(arr)) { putchar('1'); i=i+1; } return 0;}";

        //arr[0]=0; arr[1]=1; seems to be causing problems later on
        final String main = " ()~>PInt main{ [PInt] arr=new(2); arr[0]=0; arr[1]=1; printarr(arr); return 0;}";

        final String source=" "+main+" "+printarr+" ";
        final Process pr = compile_and_run_program_for_testing_with_cmd_args(source,"MainTest223",new String[0]);

        assertEquals(0,pr.exitValue());
        assertEquals("11",IOUtils.toString(pr.getInputStream()));
    }
}
